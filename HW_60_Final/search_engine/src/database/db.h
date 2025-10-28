#pragma once
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <memory>

struct Document {
    int id;
    std::string url;
    std::string title;
};

struct SearchResult {
    std::string url;
    std::string title;
    int total_score;
};

class Database {
public:
    Database();
    ~Database();
    
    bool connect();
    void initializeSchema();
    
    int insertDocument(const std::string& url, const std::string& title);
    void insertWordFrequency(int doc_id, const std::string& word, int frequency);
    bool documentExists(const std::string& url);
    std::vector<SearchResult> search(const std::vector<std::string>& words);
    
private:
    std::unique_ptr<pqxx::connection> conn_;
};