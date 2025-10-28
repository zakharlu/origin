#pragma once
#include <string>
#include <queue>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include "../database/db.h"
#include <winsock2.h>

struct CrawlTask {
    std::string url;
    int depth;
};

class Spider {
public:
    Spider(Database& db);
    ~Spider();
    
    void start();
    void stop();
    bool isFinished() const { return finished_; }
    bool isQueueEmpty() const;
    bool isWorking() const;
    int getVisitedCount() const { return visited_count_; }
    
private:
    void workerThread();
    void processURL(const std::string& url, int depth);
    std::string downloadPage(const std::string& url);
    
    Database& db_;
    std::atomic<bool> running_{false};
    std::atomic<bool> finished_{false};
    std::atomic<int> visited_count_{0};
    std::atomic<int> active_tasks_{0};
    
    std::queue<CrawlTask> queue_;
    std::unordered_set<std::string> visited_urls_;
    mutable std::mutex queue_mutex_;
    mutable std::mutex console_mutex_; // Для синхронизации вывода в консоль
    std::condition_variable condition_;
    
    std::vector<std::thread> workers_;
    int max_depth_;
    int thread_count_;
    std::string start_url_;
};