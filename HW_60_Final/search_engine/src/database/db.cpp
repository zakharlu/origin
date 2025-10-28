#include "db.h"
#include "../config/config.h"
#include <iostream>
#include <sstream>

Database::Database() {
    connect();
}

Database::~Database() {
    if (conn_ && conn_->is_open()) {
        conn_->close();
    }
}

bool Database::connect() {
    try {
        Config& config = Config::getInstance();
        
        std::string conn_str = 
            "host=" + config.getString("database", "host") +
            " port=" + std::to_string(config.getInt("database", "port")) +
            " dbname=" + config.getString("database", "dbname") +
            " user=" + config.getString("database", "user") +
            " password=" + config.getString("database", "password");
            
        conn_ = std::make_unique<pqxx::connection>(conn_str);
        
        if (conn_->is_open()) {
            std::cout << "Connected to database successfully" << std::endl;
            initializeSchema();
            return true;
        }
    } catch (const std::exception& e) {
        std::cerr << "Database connection failed: " << e.what() << std::endl;
    }
    
    return false;
}

void Database::initializeSchema() {
    try {
        pqxx::work txn(*conn_);
        
        txn.exec("CREATE TABLE IF NOT EXISTS documents ("
                 "id SERIAL PRIMARY KEY,"
                 "url TEXT UNIQUE NOT NULL,"
                 "title TEXT,"
                 "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");
        
        txn.exec("CREATE TABLE IF NOT EXISTS words ("
                 "id SERIAL PRIMARY KEY,"
                 "word TEXT UNIQUE NOT NULL)");
        
        txn.exec("CREATE TABLE IF NOT EXISTS word_frequencies ("
                 "document_id INTEGER REFERENCES documents(id) ON DELETE CASCADE,"
                 "word_id INTEGER REFERENCES words(id) ON DELETE CASCADE,"
                 "frequency INTEGER NOT NULL,"
                 "PRIMARY KEY (document_id, word_id))");
        
        txn.exec("CREATE INDEX IF NOT EXISTS idx_words_word ON words(word)");
        txn.exec("CREATE INDEX IF NOT EXISTS idx_word_frequencies_word_id ON word_frequencies(word_id)");
        txn.exec("CREATE INDEX IF NOT EXISTS idx_word_frequencies_doc_id ON word_frequencies(document_id)");
        
        txn.commit();
        std::cout << "Database schema initialized successfully" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Schema initialization failed: " << e.what() << std::endl;
    }
}

int Database::insertDocument(const std::string& url, const std::string& title) {
    try {
        // Используем отдельную транзакцию для каждой операции
        pqxx::work txn(*conn_);
        
        // Проверяем, существует ли документ
        pqxx::result exists_result = txn.exec_params(
            "SELECT id FROM documents WHERE url = $1",
            url
        );
        
        int doc_id = -1;
        
        if (!exists_result.empty()) {
            // Документ существует - обновляем
            doc_id = exists_result[0][0].as<int>();
            txn.exec_params(
                "UPDATE documents SET title = $1 WHERE id = $2",
                title, doc_id
            );
        } else {
            // Документ не существует - вставляем новый
            pqxx::result insert_result = txn.exec_params(
                "INSERT INTO documents (url, title) VALUES ($1, $2) RETURNING id",
                url, title
            );
            doc_id = insert_result[0][0].as<int>();
        }
        
        txn.commit();
        return doc_id;
        
    } catch (const std::exception& e) {
        std::cerr << "Failed to insert/update document '" << url << "': " << e.what() << std::endl;
        return -1;
    }
}

void Database::insertWordFrequency(int doc_id, const std::string& word, int frequency) {
    if (doc_id == -1) return; // Пропускаем если документ не был вставлен
    
    try {
        // Отдельная транзакция для частот слов
        pqxx::work txn(*conn_);
        
        // Вставляем слово если его нет
        txn.exec_params(
            "INSERT INTO words (word) VALUES ($1) ON CONFLICT (word) DO NOTHING",
            word
        );
        
        // Получаем ID слова
        pqxx::result word_result = txn.exec_params(
            "SELECT id FROM words WHERE word = $1",
            word
        );
        
        if (!word_result.empty()) {
            int word_id = word_result[0][0].as<int>();
            
            // Вставляем или обновляем частоту
            txn.exec_params(
                "INSERT INTO word_frequencies (document_id, word_id, frequency) "
                "VALUES ($1, $2, $3) "
                "ON CONFLICT (document_id, word_id) DO UPDATE SET frequency = EXCLUDED.frequency",
                doc_id, word_id, frequency
            );
        }
        
        txn.commit();
        
    } catch (const std::exception& e) {
        std::cerr << "Failed to insert word frequency: " << e.what() << std::endl;
    }
}

bool Database::documentExists(const std::string& url) {
    try {
        pqxx::work txn(*conn_);
        pqxx::result result = txn.exec_params(
            "SELECT 1 FROM documents WHERE url = $1",
            url
        );
        bool exists = !result.empty();
        txn.commit();
        return exists;
    } catch (const std::exception& e) {
        std::cerr << "Failed to check document existence: " << e.what() << std::endl;
        return false;
    }
}

std::vector<SearchResult> Database::search(const std::vector<std::string>& words) {
    std::vector<SearchResult> results;
    
    if (words.empty()) return results;
    
    try {
        pqxx::work txn(*conn_);
        
        // Строим параметризованный запрос
        std::string query = 
            "SELECT d.url, d.title, SUM(wf.frequency) as total_score "
            "FROM documents d "
            "JOIN word_frequencies wf ON d.id = wf.document_id "
            "JOIN words w ON wf.word_id = w.id ";
        
        // Добавляем WHERE с параметрами
        query += "WHERE w.word IN (";
        for (size_t i = 0; i < words.size(); ++i) {
            query += "$" + std::to_string(i + 1);
            if (i < words.size() - 1) query += ", ";
        }
        query += ") ";
        
        query += 
            "GROUP BY d.url, d.title "
            "HAVING COUNT(DISTINCT w.word) = $" + std::to_string(words.size() + 1) + " "
            "ORDER BY total_score DESC "
            "LIMIT 10";
        
        // Подготавливаем параметры
        std::vector<std::string> params;
        for (const auto& word : words) {
            params.push_back(word);
        }
        params.push_back(std::to_string(words.size()));
        
        // Выполняем параметризованный запрос
        pqxx::result db_results = txn.exec_params(query, pqxx::prepare::make_dynamic_params(params));
        
        for (const auto& row : db_results) {
            SearchResult result;
            result.url = row["url"].as<std::string>();
            result.title = row["title"].as<std::string>();
            result.total_score = row["total_score"].as<int>();
            results.push_back(result);
        }
        
        txn.commit();
        
    } catch (const std::exception& e) {
        std::cerr << "Search failed: " << e.what() << std::endl;
    }
    
    return results;
}