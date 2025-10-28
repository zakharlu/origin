#include "server.h"
#include "../config/config.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

#ifdef USE_BOOST_BEAST

SearchServer::SearchServer(Database& db, int port)
    : db_(db), port_(port), ioc_(1) {
}

SearchServer::~SearchServer() {
    stop();
}

void SearchServer::start() {
    if (running_) return;

    running_ = true;
    server_thread_ = std::thread(&SearchServer::run, this);
    std::cout << "Search server starting on port " << port_ << std::endl;
}

void SearchServer::stop() {
    if (!running_) return;

    running_ = false;
    ioc_.stop();
    if (server_thread_.joinable()) {
        server_thread_.join();
    }

    // Очищаем acceptor
    if (acceptor_) {
        acceptor_->close();
        acceptor_.reset();
    }

    std::cout << "Search server stopped" << std::endl;
}

void SearchServer::run() {
    try {
        auto const address = net::ip::make_address("0.0.0.0");
        auto const endpoint = tcp::endpoint{ address, static_cast<unsigned short>(port_) };

        acceptor_ = std::make_unique<tcp::acceptor>(ioc_, endpoint);

        std::cout << "Search server running on http://localhost:" << port_ << std::endl;
        std::cout << "Using Boost.Beast HTTP server" << std::endl;

        do_accept();
        ioc_.run();

    }
    catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        running_ = false;
    }
}

void SearchServer::do_accept() {
    acceptor_->async_accept(
        [this](beast::error_code ec, tcp::socket socket) {
            if (!ec) {
                // Создаем новые буфер и запрос для каждого соединения
                auto buffer_ptr = std::make_shared<beast::flat_buffer>();
                auto request_ptr = std::make_shared<http::request<http::string_body>>();

                // Используем shared_ptr для socket чтобы обеспечить правильное время жизни
                auto socket_ptr = std::make_shared<tcp::socket>(std::move(socket));

                http::async_read(*socket_ptr, *buffer_ptr, *request_ptr,
                    [this, socket_ptr, buffer_ptr, request_ptr]
                (beast::error_code ec, std::size_t bytes_transferred) {
                        if (!ec) {
                            handle_request(std::move(*request_ptr), std::move(*socket_ptr));
                        }
                        // Продолжаем принимать новые соединения
                        if (running_) {
                            do_accept();
                        }
                    });
            }
            else if (running_) {
                do_accept();
            }
        });
}

void SearchServer::handle_request(http::request<http::string_body>&& req, tcp::socket socket) {
    // Используем shared_ptr для socket чтобы обеспечить правильное время жизни
    auto socket_ptr = std::make_shared<tcp::socket>(std::move(socket));

    auto send_response = [socket_ptr](http::response<http::string_body>&& res) {
        auto res_ptr = std::make_shared<http::response<http::string_body>>(std::move(res));
        http::async_write(*socket_ptr, *res_ptr,
            [socket_ptr, res_ptr](beast::error_code ec, std::size_t) {
                // socket и response автоматически уничтожаются когда выходят из области видимости
            });
        };

    try {
        http::response<http::string_body> res;
        res.version(req.version());
        res.set(http::field::server, "SearchEngine/1.0");
        res.set(http::field::content_type, "text/html; charset=utf-8");

        if (req.method() == http::verb::get) {
            if (req.target() == "/" || req.target() == "/index.html") {
                res.result(http::status::ok);
                res.body() = get_search_page();
            }
            else {
                res.result(http::status::not_found);
                res.body() = get_error_page("Page Not Found");
            }
        }
        else if (req.method() == http::verb::post) {
            if (req.target() == "/search") {
                res.result(http::status::ok);
                res.body() = handle_post_request(req.body());
            }
            else {
                res.result(http::status::not_found);
                res.body() = get_error_page("Page Not Found");
            }
        }
        else {
            res.result(http::status::method_not_allowed);
            res.set(http::field::allow, "GET, POST");
            res.body() = get_error_page("Method Not Allowed");
        }

        res.prepare_payload();
        send_response(std::move(res));

    }
    catch (const std::exception& e) {
        std::cerr << "Request handling error: " << e.what() << std::endl;

        http::response<http::string_body> res;
        res.result(http::status::internal_server_error);
        res.set(http::field::content_type, "text/html; charset=utf-8");
        res.body() = get_error_page("Internal Server Error");
        res.prepare_payload();
        send_response(std::move(res));
    }
}

#else
// Реализация с cpp-httplib
#include "../../lib/httplib.h"

SearchServer::SearchServer(Database& db, int port)
    : db_(db), port_(port) {
}

SearchServer::~SearchServer() {
    stop();
}

void SearchServer::start() {
    if (running_) return;

    running_ = true;
    server_thread_ = std::thread(&SearchServer::run, this);
    std::cout << "Search server starting on port " << port_ << std::endl;
}

void SearchServer::stop() {
    if (!running_) return;

    running_ = false;
    if (server_thread_.joinable()) {
        server_thread_.join();
    }
    std::cout << "Search server stopped" << std::endl;
}

void SearchServer::run() {
    httplib::Server server;

    // GET / - главная страница с формой поиска
    server.Get("/", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content(get_search_page(), "text/html; charset=utf-8");
        });

    // POST /search - обработка поискового запроса
    server.Post("/search", [this](const httplib::Request& req, httplib::Response& res) {
        try {
            std::string query;

            if (req.has_param("q")) {
                query = req.get_param_value("q");
            }
            else if (req.body.find("q=") != std::string::npos) {
                size_t pos = req.body.find("q=");
                if (pos != std::string::npos) {
                    query = req.body.substr(pos + 2);
                    size_t amp_pos = query.find('&');
                    if (amp_pos != std::string::npos) {
                        query = query.substr(0, amp_pos);
                    }
                    query = url_decode(query);
                }
            }

            if (query.empty()) {
                res.set_content(get_search_page(), "text/html; charset=utf-8");
                return;
            }

            std::vector<std::string> words = parse_query(query);

            if (words.empty()) {
                res.set_content(get_results_page({}, query), "text/html; charset=utf-8");
                return;
            }

            std::vector<SearchResult> results = db_.search(words);
            res.set_content(get_results_page(results, query), "text/html; charset=utf-8");

        }
        catch (const std::exception& e) {
            std::cerr << "Search error: " << e.what() << std::endl;
            res.set_content(get_error_page("Internal Server Error"), "text/html; charset=utf-8");
        }
        });

    server.set_error_handler([](const httplib::Request& req, httplib::Response& res) {
        res.set_content(get_error_page("Page Not Found"), "text/html; charset=utf-8");
        });

    std::cout << "Search server running on http://localhost:" << port_ << std::endl;
    std::cout << "Using cpp-httplib HTTP server" << std::endl;

    if (!server.listen("0.0.0.0", port_)) {
        std::cerr << "Failed to start search server on port " << port_ << std::endl;
        running_ = false;
    }
}
#endif

// Общие функции (одинаковые для обеих реализаций)
std::string SearchServer::handle_post_request(const std::string& body) {
    std::string query;

    size_t q_pos = body.find("q=");
    if (q_pos != std::string::npos) {
        query = body.substr(q_pos + 2);
        size_t amp_pos = query.find('&');
        if (amp_pos != std::string::npos) {
            query = query.substr(0, amp_pos);
        }
        query = url_decode(query);
    }

    if (query.empty()) {
        return get_search_page();
    }

    std::vector<std::string> words = parse_query(query);

    if (words.empty()) {
        return get_results_page({}, query);
    }

    std::vector<SearchResult> results = db_.search(words);
    return get_results_page(results, query);
}

std::vector<std::string> SearchServer::parse_query(const std::string& query) {
    std::vector<std::string> words;
    std::istringstream iss(query);
    std::string word;

    while (iss >> word && words.size() < 4) {
        std::string clean_word;
        for (char c : word) {
            if (std::isalnum(c)) {
                clean_word += std::tolower(c);
            }
        }

        if (clean_word.length() >= 3) {
            words.push_back(clean_word);
        }
    }

    return words;
}

std::string SearchServer::url_decode(const std::string& encoded) {
    std::string decoded;
    for (size_t i = 0; i < encoded.size(); ++i) {
        if (encoded[i] == '%' && i + 2 < encoded.size()) {
            int hex_val;
            std::stringstream ss;
            ss << std::hex << encoded.substr(i + 1, 2);
            ss >> hex_val;
            decoded += static_cast<char>(hex_val);
            i += 2;
        }
        else if (encoded[i] == '+') {
            decoded += ' ';
        }
        else {
            decoded += encoded[i];
        }
    }
    return decoded;
}

// HTML страница с формой поиска
std::string SearchServer::get_search_page() {
    return R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Search Engine</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
        }
        
        .search-container {
            background: white;
            padding: 3rem;
            border-radius: 20px;
            box-shadow: 0 20px 40px rgba(0,0,0,0.1);
            text-align: center;
            max-width: 600px;
            width: 90%;
        }
        
        .logo {
            font-size: 3rem;
            margin-bottom: 1rem;
            color: #667eea;
        }
        
        h1 {
            color: #333;
            margin-bottom: 2rem;
            font-size: 2.5rem;
            font-weight: 300;
        }
        
        .search-form {
            display: flex;
            gap: 10px;
            margin-bottom: 1rem;
        }
        
        .search-input {
            flex: 1;
            padding: 15px 20px;
            font-size: 1.1rem;
            border: 2px solid #e1e5e9;
            border-radius: 50px;
            outline: none;
            transition: all 0.3s ease;
        }
        
        .search-input:focus {
            border-color: #667eea;
            box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.1);
        }
        
        .search-button {
            padding: 15px 30px;
            background: #667eea;
            color: white;
            border: none;
            border-radius: 50px;
            font-size: 1.1rem;
            cursor: pointer;
            transition: background 0.3s ease;
        }
        
        .search-button:hover {
            background: #5a6fd8;
        }
        
        .tips {
            color: #666;
            font-size: 0.9rem;
            margin-top: 1rem;
        }
    </style>
</head>
<body>
    <div class="search-container">
        <div class="logo">🔍</div>
        <h1>Search Engine</h1>
        <form method="POST" action="/search" class="search-form">
            <input type="text" name="q" class="search-input" placeholder="Enter your search query..." required>
            <button type="submit" class="search-button">Search</button>
        </form>
        <div class="tips">
            Enter up to 4 words. Each word should be at least 3 characters long.
        </div>
    </div>
</body>
</html>)";
}

// HTML страница с результатами поиска
std::string SearchServer::get_results_page(const std::vector<SearchResult>& results, const std::string& query) {
    std::ostringstream html;

    html << R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Results for: )" << query << R"( - Search Engine</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: #f8f9fa;
            color: #333;
            line-height: 1.6;
        }
        
        .header {
            background: white;
            padding: 1.5rem 2rem;
            box-shadow: 0 2px 10px rgba(0,0,0,0.1);
            border-bottom: 1px solid #e1e5e9;
        }
        
        .header-content {
            max-width: 1200px;
            margin: 0 auto;
            display: flex;
            align-items: center;
            gap: 2rem;
        }
        
        .logo {
            font-size: 1.5rem;
            color: #667eea;
            text-decoration: none;
        }
        
        .search-form {
            flex: 1;
            display: flex;
            gap: 10px;
            max-width: 600px;
        }
        
        .search-input {
            flex: 1;
            padding: 12px 20px;
            font-size: 1rem;
            border: 2px solid #e1e5e9;
            border-radius: 50px;
            outline: none;
        }
        
        .search-input:focus {
            border-color: #667eea;
        }
        
        .search-button {
            padding: 12px 24px;
            background: #667eea;
            color: white;
            border: none;
            border-radius: 50px;
            cursor: pointer;
        }
        
        .main-content {
            max-width: 1200px;
            margin: 0 auto;
            padding: 2rem;
        }
        
        .results-info {
            color: #666;
            margin-bottom: 2rem;
            font-size: 1.1rem;
        }
        
        .results-count {
            color: #667eea;
            font-weight: 600;
        }
        
        .results-list {
            display: flex;
            flex-direction: column;
            gap: 1.5rem;
        }
        
        .result-item {
            background: white;
            padding: 1.5rem;
            border-radius: 10px;
            box-shadow: 0 2px 10px rgba(0,0,0,0.05);
            border: 1px solid #e1e5e9;
            transition: transform 0.2s ease, box-shadow 0.2s ease;
        }
        
        .result-item:hover {
            transform: translateY(-2px);
            box-shadow: 0 4px 20px rgba(0,0,0,0.1);
        }
        
        .result-title {
            font-size: 1.3rem;
            margin-bottom: 0.5rem;
        }
        
        .result-title a {
            color: #1a0dab;
            text-decoration: none;
        }
        
        .result-title a:hover {
            text-decoration: underline;
        }
        
        .result-url {
            color: #006621;
            font-size: 0.9rem;
            margin-bottom: 0.5rem;
            word-break: break-all;
        }
        
        .result-score {
            color: #70757a;
            font-size: 0.8rem;
        }
        
        .no-results {
            text-align: center;
            padding: 3rem;
            color: #666;
        }
        
        .no-results .icon {
            font-size: 4rem;
            margin-bottom: 1rem;
            opacity: 0.5;
        }
        
        .suggestions {
            margin-top: 1rem;
            color: #666;
        }
        
        @media (max-width: 768px) {
            .header-content {
                flex-direction: column;
                gap: 1rem;
            }
            
            .search-form {
                width: 100%;
            }
            
            .main-content {
                padding: 1rem;
            }
        }
    </style>
</head>
<body>
    <div class="header">
        <div class="header-content">
            <a href="/" class="logo">🔍 Search Engine</a>
            <form method="POST" action="/search" class="search-form">
                <input type="text" name="q" class="search-input" value=")" << query << R"(" placeholder="Search...">
                <button type="submit" class="search-button">Search</button>
            </form>
        </div>
    </div>
    
    <div class="main-content">)";

    if (results.empty()) {
        html << R"(
        <div class="no-results">
            <div class="icon">🔍</div>
            <h2>No results found</h2>
            <p>Your search for "<strong>)" << query << R"(</strong>" did not match any documents.</p>
            <div class="suggestions">
                <p>Suggestions:</p>
                <ul>
                    <li>Make sure all words are spelled correctly</li>
                    <li>Try different keywords</li>
                    <li>Try more general keywords</li>
                    <li>Try fewer keywords</li>
                </ul>
            </div>
        </div>)";
    }
    else {
        html << R"(
        <div class="results-info">
            Found <span class="results-count">)" << results.size() << R"(</span> results for "<strong>)" << query << R"(</strong>"
        </div>
        
        <div class="results-list">)";

        for (const auto& result : results) {
            html << R"(
            <div class="result-item">
                <div class="result-title">
                    <a href=")" << result.url << R"(" target="_blank">)" << result.title << R"(</a>
                </div>
                <div class="result-url">)" << result.url << R"(</div>
                <div class="result-score">Relevance score: )" << result.total_score << R"(</div>
            </div>)";
        }

        html << R"(
        </div>)";
    }

    html << R"(
    </div>
</body>
</html>)";

    return html.str();
}

// HTML страница с ошибкой
std::string SearchServer::get_error_page(const std::string& message) {
    return R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Error - Search Engine</title>
    <style>
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            align-items: center;
            justify-content: center;
            margin: 0;
            padding: 1rem;
        }
        
        .error-container {
            background: white;
            padding: 3rem;
            border-radius: 20px;
            box-shadow: 0 20px 40px rgba(0,0,0,0.1);
            text-align: center;
            max-width: 500px;
            width: 100%;
        }
        
        .error-icon {
            font-size: 4rem;
            margin-bottom: 1rem;
            color: #ff6b6b;
        }
        
        h1 {
            color: #333;
            margin-bottom: 1rem;
        }
        
        .error-message {
            color: #666;
            margin-bottom: 2rem;
            line-height: 1.6;
        }
        
        .home-link {
            display: inline-block;
            padding: 12px 24px;
            background: #667eea;
            color: white;
            text-decoration: none;
            border-radius: 50px;
            transition: background 0.3s ease;
        }
        
        .home-link:hover {
            background: #5a6fd8;
        }
    </style>
</head>
<body>
    <div class="error-container">
        <div class="error-icon">⚠️</div>
        <h1>Error</h1>
        <div class="error-message">)" + message + R"(</div>
        <a href="/" class="home-link">Back to Search</a>
    </div>
</body>
</html>)";
}