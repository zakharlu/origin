#include <iostream>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <atomic>
#include <chrono>

// Шаблонный класс потокобезопасной очереди
template<typename T>
class safe_queue {
private:
    std::queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_var_;
    std::atomic<bool> stop_{ false };

public:
    safe_queue() = default;

    // Запрещаем копирование и присваивание
    safe_queue(const safe_queue&) = delete;
    safe_queue& operator=(const safe_queue&) = delete;

    // Помещение элемента в очередь
    void push(T item) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(item));
        }
        cond_var_.notify_one();
    }

    // Извлечение элемента из очереди (с ожиданием)
    bool pop(T& item) {
        std::unique_lock<std::mutex> lock(mutex_);

        // Ожидаем, пока не появится элемент или не будет остановки
        cond_var_.wait(lock, [this]() {
            return !queue_.empty() || stop_.load();
            });

        if (stop_.load() && queue_.empty()) {
            return false;
        }

        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    // Попытка извлечения без ожидания
    bool try_pop(T& item) {
        std::lock_guard<std::mutex> lock(mutex_);

        if (queue_.empty()) {
            return false;
        }

        item = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    // Проверка на пустоту
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    // Получение размера очереди
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

    // Остановка очереди
    void stop() {
        stop_.store(true);
        cond_var_.notify_all();
    }
};

// Класс пула потоков
class thread_pool {
private:
    safe_queue<std::function<void()>> tasks_queue_;
    std::vector<std::thread> workers_;
    std::atomic<bool> stop_{ false };
    std::atomic<int> active_tasks_{ 0 };

public:
    thread_pool() {
        // Создаем количество потоков равное количеству аппаратных ядер
        const size_t thread_count = std::thread::hardware_concurrency();
        std::cout << "Создан пул потоков с " << thread_count << " потоками\n";

        workers_.reserve(thread_count);
        for (size_t i = 0; i < thread_count; ++i) {
            workers_.emplace_back(&thread_pool::work, this);
        }
    }

    ~thread_pool() {
        stop();

        // Ожидаем завершения всех рабочих потоков
        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    // Запрещаем копирование и присваивание
    thread_pool(const thread_pool&) = delete;
    thread_pool& operator=(const thread_pool&) = delete;

    // Метод работы потока
    void work() {
        while (!stop_.load()) {
            std::function<void()> task;
            if (tasks_queue_.pop(task)) {
                ++active_tasks_;
                try {
                    task();
                }
                catch (const std::exception& e) {
                    std::cerr << "Исключение в пуле потоков: " << e.what() << std::endl;
                }
                --active_tasks_;
            }
        }
    }

    // Добавление задачи в пул
    template<typename F>
    auto submit(F&& f) -> std::future<decltype(f())> {
        using return_type = decltype(f());

        // Создаем packaged_task для получения future
        auto task_ptr = std::make_shared<std::packaged_task<return_type()>>(
            std::forward<F>(f)
        );

        // Оборачиваем в function<void()> для хранения в очереди
        std::function<void()> wrapper_func = [task_ptr]() {
            (*task_ptr)();
            };

        tasks_queue_.push(std::move(wrapper_func));

        return task_ptr->get_future();
    }

    // Простая версия submit для function<void()>
    void submit(const std::function<void()>& task) {
        tasks_queue_.push(task);
    }

    // Остановка пула
    void stop() {
        stop_.store(true);
        tasks_queue_.stop();
    }

    // Получение количества активных задач
    int active_tasks() const {
        return active_tasks_.load();
    }

    // Получение размера очереди задач
    size_t queue_size() const {
        return tasks_queue_.size();
    }
};

// Тестовые функции, которые выводят только своё имя
void функция_А() {
    std::cout << "функция_А" << std::endl;
}

void функция_Б() {
    std::cout << "функция_Б" << std::endl;
}

void функция_В() {
    std::cout << "функция_В" << std::endl;
}

void функция_Г() {
    std::cout << "функция_Г" << std::endl;
}

void функция_Д() {
    std::cout << "функция_Д" << std::endl;
}

void функция_Е() {
    std::cout << "функция_Е" << std::endl;
}

int main() {
    // Устанавливаем локаль для поддержки русского языка
    std::locale::global(std::locale(""));
    std::cout.imbue(std::locale());

    std::cout << "=== Демонстрация пула потоков ===" << std::endl;

    // Создаем пул потоков
    thread_pool pool;

    // Даем время на инициализацию потоков
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // Выполняем тесты в течение 10 секунд
    auto время_начала = std::chrono::steady_clock::now();
    int итерация = 0;

    while (std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::steady_clock::now() - время_начала).count() < 10) {

        std::cout << "\n--- Итерация " << ++итерация << " ---" << std::endl;
        std::cout << "Активные задачи: " << pool.active_tasks()
            << ", Задач в очереди: " << pool.queue_size() << std::endl;

        // Каждую секунду помещаем в пул одновременно 2 функции
        std::cout << "Добавляем 2 функции в пул:" << std::endl;

        // Первая пара функций
        pool.submit(функция_А);
        pool.submit(функция_Б);

        // Ждем немного чтобы увидеть выполнение
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        std::cout << "Состояние после добавления: Активные задачи: " << pool.active_tasks()
            << ", Задач в очереди: " << pool.queue_size() << std::endl;

        // Ждем 1 секунду перед следующей итерацией
        std::this_thread::sleep_for(std::chrono::milliseconds(900));
    }

    // Дополнительная демонстрация - добавляем несколько функций подряд
    std::cout << "\n=== Дополнительная демонстрация ===" << std::endl;
    std::cout << "Добавляем несколько функций одновременно:" << std::endl;

    pool.submit(функция_В);
    pool.submit(функция_Г);
    pool.submit(функция_Д);
    pool.submit(функция_Е);
    pool.submit(функция_А);
    pool.submit(функция_Б);

    // Ждем завершения всех задач
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "\n=== Демонстрация завершена ===" << std::endl;
    std::cout << "Финальное состояние: Активные задачи: " << pool.active_tasks()
        << ", Задач в очереди: " << pool.queue_size() << std::endl;

    return 0;
}