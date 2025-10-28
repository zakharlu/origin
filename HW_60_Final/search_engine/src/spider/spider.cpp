#include "spider.h"
#include "../config/config.h"
#include "../parser/html_parser.h"
#include "../../lib/httplib.h"
#include <iostream>
#include <sstream>
#include <thread>
#include <regex>
#include <algorithm>

// Функция для скачивания страниц с полной поддержкой HTTPS и HTTP
std::string downloadHTTP(const std::string& url) {
    try {
        // Парсим URL
        std::regex url_regex(R"(^(https?)://([^/]+)(/.*)?$)");
        std::smatch match;

        if (!std::regex_match(url, match, url_regex)) {
            std::cerr << "Invalid URL: " << url << std::endl;
            return "";
        }

        std::string protocol = match[1].str();
        std::string host = match[2].str();
        std::string path = match[3].str().empty() ? "/" : match[3].str();

        std::cout << "Connecting to: " << host << " via " << protocol << std::endl;

        // Настройки клиента
        const int timeout_seconds = 10;
        const bool follow_redirects = true;

        if (protocol == "https") {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
            try {
                // HTTPS клиент
                httplib::SSLClient https_client(host);
                https_client.set_follow_location(follow_redirects);
                https_client.set_connection_timeout(timeout_seconds);
                https_client.set_read_timeout(timeout_seconds);
                https_client.enable_server_certificate_verification(false); // Для тестирования

                httplib::Headers headers = {
                    {"User-Agent", "Mozilla/5.0 (compatible; SearchEngineSpider/1.0)"},
                    {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"},
                    {"Accept-Language", "en-US,en;q=0.5"},
                    {"Accept-Encoding", "identity"},
                    {"Connection", "close"}
                };

                auto response = https_client.Get(path.c_str(), headers);

                if (response) {
                    std::cout << "HTTPS response status: " << response->status << std::endl;

                    if (response->status == 200) {
                        std::cout << "Successfully downloaded " << response->body.length() << " bytes via HTTPS" << std::endl;
                        return response->body;
                    }
                    else if (response->status >= 300 && response->status < 400) {
                        std::cout << "Redirect detected: " << response->status << std::endl;
                        return "";
                    }
                    else {
                        std::cerr << "HTTPS request failed. Status: " << response->status << std::endl;
                        return "";
                    }
                }
                else {
                    std::cerr << "HTTPS request failed - no response" << std::endl;
                    return "";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "HTTPS client exception: " << e.what() << std::endl;
                return "";
            }
#else
            std::cerr << "HTTPS not supported in this build. Recompile with OpenSSL." << std::endl;
            return "";
#endif
        }
        else {
            // HTTP клиент
            httplib::Client http_client(host);
            http_client.set_follow_location(follow_redirects);
            http_client.set_connection_timeout(timeout_seconds);
            http_client.set_read_timeout(timeout_seconds);

            httplib::Headers headers = {
                {"User-Agent", "Mozilla/5.0 (compatible; SearchEngineSpider/1.0)"},
                {"Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"},
                {"Accept-Language", "en-US,en;q=0.5"},
                {"Accept-Encoding", "identity"},
                {"Connection", "close"}
            };

            auto response = http_client.Get(path.c_str(), headers);

            if (response) {
                std::cout << "HTTP response status: " << response->status << std::endl;

                if (response->status == 200) {
                    std::cout << "Successfully downloaded " << response->body.length() << " bytes via HTTP" << std::endl;
                    return response->body;
                }
                else {
                    std::cerr << "HTTP request failed. Status: " << response->status << std::endl;
                    return "";
                }
            }
            else {
                std::cerr << "HTTP request failed - no response" << std::endl;
                return "";
            }
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Exception during download of " << url << ": " << e.what() << std::endl;
        return "";
    }
    catch (...) {
        std::cerr << "Unknown exception during download of " << url << std::endl;
        return "";
    }
}

Spider::Spider(Database& db) : db_(db) {
    Config& config = Config::getInstance();
    start_url_ = config.getString("spider", "start_url");
    max_depth_ = config.getInt("spider", "max_depth");
    thread_count_ = config.getInt("spider", "thread_count");

    std::cout << "Spider initialized:" << std::endl;
    std::cout << "  Start URL: " << start_url_ << std::endl;
    std::cout << "  Max depth: " << max_depth_ << std::endl;
    std::cout << "  Threads: " << thread_count_ << std::endl;
}

Spider::~Spider() {
    stop();
}

void Spider::start() {
    if (running_) return;

    running_ = true;
    finished_ = false;
    visited_count_ = 0;
    active_tasks_ = 0;

    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        queue_.push({ start_url_, 1 }); // Начинаем с глубины 1
        visited_urls_.insert(start_url_);
    }

    // Запускаем рабочие потоки
    for (int i = 0; i < thread_count_; ++i) {
        workers_.emplace_back(&Spider::workerThread, this);
    }

    std::cout << "Spider started with " << thread_count_ << " threads" << std::endl;
    std::cout << "Start URL: " << start_url_ << std::endl;
    std::cout << "Max depth: " << max_depth_ << std::endl;
    std::cout << "Depth levels to index: ";
    for (int i = 1; i <= max_depth_; ++i) {
        std::cout << i;
        if (i < max_depth_) std::cout << ", ";
    }
    std::cout << std::endl;
}

void Spider::stop() {
    if (!running_) return;

    std::cout << "Stopping spider..." << std::endl;
    running_ = false;
    condition_.notify_all();

    for (auto& thread : workers_) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    workers_.clear();
    finished_ = true;
    std::cout << "Spider stopped. Total URLs processed: " << visited_count_ << std::endl;
}

bool Spider::isQueueEmpty() const {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    return queue_.empty();
}

bool Spider::isWorking() const {
    return running_ && (!isQueueEmpty() || active_tasks_ > 0);
}

void Spider::workerThread() {
    std::cout << "Spider worker thread started" << std::endl;

    while (running_) {
        CrawlTask task;
        bool has_work = false;

        {
            std::unique_lock<std::mutex> lock(queue_mutex_);

            // Ждем работу или остановку
            condition_.wait(lock, [this]() {
                return !queue_.empty() || !running_;
                });

            if (!running_) break;

            if (!queue_.empty()) {
                task = queue_.front();
                queue_.pop();
                has_work = true;
            }
        }

        if (has_work) {
            active_tasks_++;
            processURL(task.url, task.depth);
            active_tasks_--;
        }
    }

    std::cout << "Spider worker thread finished" << std::endl;
}

void Spider::processURL(const std::string& url, int depth) {
    if (!running_) return;

    std::cout << "\n=== Processing: " << url << " (depth: " << depth << ") ===" << std::endl;

    try {
        // Добавляем задержку чтобы не перегружать сервер
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::string html = downloadPage(url);

        if (!html.empty() && running_) {
            ParsedPage page = HTMLParser::parse(html, url);

            // Вставляем документ в базу данных
            int doc_id = db_.insertDocument(url, page.title);
            if (doc_id != -1 && running_) {
                // Вставляем частоты слов
                for (const auto& [word, freq] : page.word_frequencies) {
                    if (!running_) break;
                    db_.insertWordFrequency(doc_id, word, freq);
                }
                std::cout << "[OK] Successfully indexed: " << url << std::endl;
                std::cout << "  Title: " << (page.title.empty() ? "No Title" : page.title) << std::endl;
                std::cout << "  Unique words: " << page.word_frequencies.size() << std::endl;
                std::cout << "  Links found: " << page.links.size() << std::endl;
            }
            else if (!running_) {
                std::cout << "Indexing interrupted for: " << url << std::endl;
            }

            visited_count_++;

            // Добавляем новые ссылки в очередь (если не достигнута максимальная глубина)
            int next_depth = depth + 1;
            if (next_depth <= max_depth_ && running_) {
                std::lock_guard<std::mutex> lock(queue_mutex_);
                int new_links_count = 0;

                for (const auto& link : page.links) {
                    if (!running_) break;

                    // Фильтруем только HTTP/HTTPS ссылки и проверяем, не посещали ли уже
                    if ((link.find("http://") == 0 || link.find("https://") == 0) &&
                        visited_urls_.find(link) == visited_urls_.end()) {

                        visited_urls_.insert(link);
                        queue_.push({ link, next_depth });
                        new_links_count++;
                        std::cout << "  -> Added to queue (depth " << next_depth << "): " << link << std::endl;
                    }
                }

                if (new_links_count > 0) {
                    condition_.notify_all(); // Будим другие потоки
                    std::cout << "  -> Total " << new_links_count << " new links added to queue (depth " << next_depth << ")" << std::endl;
                }

                std::cout << "  Queue size: " << queue_.size() << std::endl;
            }
            else {
                std::cout << "  -> Max depth reached (" << max_depth_ << "), skipping " << page.links.size() << " links" << std::endl;
            }
        }
        else {
            std::cerr << "[ERROR] Failed to process: " << url << " (empty content)" << std::endl;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Error processing " << url << ": " << e.what() << std::endl;
    }
}

std::string Spider::downloadPage(const std::string& url) {
    if (!running_) return "";

    try {
        std::cout << "Downloading: " << url << std::endl;

        std::string html = downloadHTTP(url);

        if (html.empty()) {
            std::cerr << "Empty content from: " << url << std::endl;
            return "";
        }

        // Проверяем размер содержимого
        if (html.length() < 50) {
            std::cout << "Small content (" << html.length() << " bytes) from: " << url << std::endl;
            // Возвращаем даже маленькое содержимое - может быть нормальной страницей
        }

        std::cout << "[OK] Downloaded " << html.length() << " bytes from " << url << std::endl;
        return html;

    }
    catch (const std::exception& e) {
        std::cerr << "Download failed for " << url << ": " << e.what() << std::endl;
        return "";
    }
}