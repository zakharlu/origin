#pragma once
#include "../database/db.h"
#include <string>
#include <thread>
#include <atomic>

#ifdef USE_BOOST_BEAST
#include <boost/asio.hpp>
#include <boost/beast.hpp>
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;
#endif

class SearchServer {
public:
    SearchServer(Database& db, int port);
    ~SearchServer();
    
    void start();
    void stop();
    bool isRunning() const { return running_; }
    
private:
    void run();
    
#ifdef USE_BOOST_BEAST
    void do_accept();
    void handle_request(http::request<http::string_body>&& req, tcp::socket socket);
    
    net::io_context ioc_;
    std::unique_ptr<tcp::acceptor> acceptor_;
#endif
    
    Database& db_;
    int port_;
    std::atomic<bool> running_{false};
    std::thread server_thread_;
    
    // Общие вспомогательные функции
    std::string handle_post_request(const std::string& body);
    std::vector<std::string> parse_query(const std::string& query);
    std::string url_decode(const std::string& encoded);
    
    // HTML генерация
    std::string get_search_page();
    std::string get_results_page(const std::vector<SearchResult>& results, const std::string& query);
    std::string get_error_page(const std::string& message);
};