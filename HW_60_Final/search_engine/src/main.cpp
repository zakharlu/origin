#include <iostream>
#include <string>
#include <atomic>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdio>
#include <conio.h>
#include "config/config.h"
#include "database/db.h"
#include "spider/spider.h"
#include "search/server.h"

std::atomic<bool> running{ true };

std::string findConfigFile() {
    std::vector<std::string> possible_paths = {
        "config.ini",
        "../config.ini",
        "../../config.ini",
    };

    for (const auto& path : possible_paths) {
        std::ifstream test_file(path);
        if (test_file.good()) {
            std::cout << "Configuration file found: " << path << std::endl;
            test_file.close();
            return path;
        }
    }

    return "";
}

void checkForExit() {
    while (running) {
        if (_kbhit()) {
            char c = _getch();
            if (c == 'q' || c == 'Q' || c == 3) {
                std::cout << "\n*** Stop command received - shutting down... ***" << std::endl;
                running = false;
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");
    setvbuf(stdout, nullptr, _IONBF, 0);


    std::cout << "=== Starting Search Engine ===" << std::endl;

    std::string config_path = findConfigFile();
    if (config_path.empty()) {
        std::cerr << "Error: config.ini file not found!" << std::endl;
        std::cerr << "Please create config.ini in the project root with the following structure:" << std::endl;
        std::cerr << R"(
[database]
host=localhost
port=5432
dbname=search_engine
user=postgres
password=your_password

[spider]
start_url=http://example.com
max_depth=3
thread_count=2

[server]
port=8080
)" << std::endl;
        std::cout << "Press Enter to exit..." << std::endl;
        std::cin.get();
        return 1;
    }

    Config& config = Config::getInstance();
    if (!config.load(config_path)) {
        std::cerr << "Failed to load configuration file: " << config_path << std::endl;
        std::cout << "Press Enter to exit..." << std::endl;
        std::cin.get();
        return 1;
    }

    std::cout << "Configuration successfully loaded from: " << config_path << std::endl;

    std::string db_host = config.getString("database", "host");
    int db_port = config.getInt("database", "port");
    std::string db_name = config.getString("database", "dbname");
    std::string db_user = config.getString("database", "user");
    std::string db_password = config.getString("database", "password");
    std::string spider_start_url = config.getString("spider", "start_url");
    int spider_max_depth = config.getInt("spider", "max_depth");
    int spider_thread_count = config.getInt("spider", "thread_count");
    int server_port = config.getInt("server", "port");

    if (spider_start_url.empty() || server_port <= 0) {
        std::cerr << "Invalid configuration parameters" << std::endl;
        return 1;
    }

    std::cout << "System Configuration:" << std::endl;
    std::cout << "  Database: " << db_host << ":" << db_port << "/" << db_name << std::endl;
    std::cout << "  Spider: " << spider_start_url << " (depth: " << spider_max_depth << ", threads: " << spider_thread_count << ")" << std::endl;
    std::cout << "  Server Port: " << server_port << std::endl;

    try {
        std::cout << "Initializing database..." << std::endl;
        Database db;
        if (!db.connect()) {
            std::cerr << "Failed to connect to database!" << std::endl;
            std::cout << "Press Enter to exit..." << std::endl;
            std::cin.get();
            return 1;
        }
        std::cout << "Database connected successfully" << std::endl;

        std::cout << "Starting search server on port " << server_port << "..." << std::endl;
        SearchServer server(db, server_port);
        server.start();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::cout << "Starting spider..." << std::endl;
        Spider spider(db);
        spider.start();

        std::cout << "=== Search Engine Successfully Started ===" << std::endl;
        std::cout << "Web Interface: http://localhost:" << server_port << std::endl;
        std::cout << "Press 'Q' to stop the system" << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        std::thread exit_thread(checkForExit);

        bool spider_completed = false;
        bool spider_started = false;

        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(2));

            if (!spider_started && spider.isWorking()) {
                std::cout << "Spider started indexing..." << std::endl;
                spider_started = true;
            }

            if (!spider_completed && !spider.isWorking() && spider.getVisitedCount() > 0) {
                std::cout << "Spider completed work! Indexed " << spider.getVisitedCount() << " pages." << std::endl;
                std::cout << "Search server continues to run..." << std::endl;
                spider_completed = true;
                spider.stop();

                std::cout << std::endl;
                std::cout << "Start search on http://localhost:8080/" << std::endl;
                std::cout << "Press 'Q' to stop the search server" << std::endl;
            }
        }

        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Shutting down search engine..." << std::endl;

        server.stop();
        if (!spider_completed) {
            spider.stop();
        }

        if (exit_thread.joinable()) {
            exit_thread.join();
        }

        std::cout << "=== Search Engine Stopped ===" << std::endl;
        std::cout << "Final Statistics:" << std::endl;
        std::cout << "  Indexed Pages: " << spider.getVisitedCount() << std::endl;
        std::cout << "  Search Server: stopped" << std::endl;
        std::cout << "  Spider: " << (spider_completed ? "completed" : "stopped") << std::endl;
        std::cout << "Goodbye!" << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Critical Error: " << e.what() << std::endl;
        running = false;
        std::cout << "Press Enter to exit..." << std::endl;
        std::cin.get();
        return 1;
    }

    return 0;
}