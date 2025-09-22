#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <vector>

class AtomicQueueSimulator {
private:
    std::atomic<int> clientCount;
    int maxClients;
    std::atomic<bool> isRunning;
    std::mutex coutMutex;

public:
    AtomicQueueSimulator(int maxClients)
        : clientCount(0), maxClients(maxClients), isRunning(true) {
    }

    // Версия с memory_order_seq_cst (последовательная согласованность)
    void clientThreadSeqCst() {
        while (isRunning.load(std::memory_order_acquire)) {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            int current = clientCount.load(std::memory_order_acquire);
            if (current < maxClients) {
                // Атомарное сравнение и обмен (CAS)
                while (!clientCount.compare_exchange_weak(
                    current,
                    current + 1,
                    std::memory_order_seq_cst,
                    std::memory_order_relaxed
                )) {
                    if (current >= maxClients) break;
                }

                if (current < maxClients) {
                    std::lock_guard<std::mutex> lock(coutMutex);
                    std::cout << "[SEQ_CST] Клиент пришел. В очереди: " << current + 1 << " клиентов" << std::endl;
                }
            }
        }
    }

    // Версия с memory_order_release/acquire
    void clientThreadReleaseAcquire() {
        while (isRunning.load(std::memory_order_acquire)) {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            int current = clientCount.load(std::memory_order_acquire);
            if (current < maxClients) {
                clientCount.fetch_add(1, std::memory_order_release);

                std::lock_guard<std::mutex> lock(coutMutex);
                std::cout << "[RELEASE] Клиент пришел. В очереди: " << current + 1 << " клиентов" << std::endl;
            }
        }
    }

    // Версия с memory_order_relaxed
    void clientThreadRelaxed() {
        while (isRunning.load(std::memory_order_relaxed)) {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            int current = clientCount.load(std::memory_order_relaxed);
            if (current < maxClients) {
                clientCount.fetch_add(1, std::memory_order_relaxed);

                std::lock_guard<std::mutex> lock(coutMutex);
                std::cout << "[RELAXED] Клиент пришел. В очереди: " << current + 1 << " клиентов" << std::endl;
            }
        }
    }

    // Операционист с различными режимами памяти
    void operatorThread(int mode) {
        while (isRunning.load(std::memory_order_acquire) ||
            clientCount.load(std::memory_order_acquire) > 0) {

            std::this_thread::sleep_for(std::chrono::seconds(2));

            int current;
            switch (mode) {
            case 0: // seq_cst
                current = clientCount.load(std::memory_order_seq_cst);
                if (current > 0) {
                    clientCount.fetch_sub(1, std::memory_order_seq_cst);
                }
                break;
            case 1: // release/acquire
                current = clientCount.load(std::memory_order_acquire);
                if (current > 0) {
                    clientCount.fetch_sub(1, std::memory_order_release);
                }
                break;
            case 2: // relaxed
                current = clientCount.load(std::memory_order_relaxed);
                if (current > 0) {
                    clientCount.fetch_sub(1, std::memory_order_relaxed);
                }
                break;
            }

            if (current > 0) {
                std::lock_guard<std::mutex> lock(coutMutex);
                std::cout << "Операционист обслужил клиента. В очереди: " << current - 1 << " клиентов" << std::endl;
            }
        }
    }

    void stop() {
        isRunning.store(false, std::memory_order_release);
    }

    int getClientCount() {
        return clientCount.load(std::memory_order_acquire);
    }

    // Тестовая функция для проверки корректности
    void stressTest(int iterations) {
        std::cout << "Запуск стресс-теста (" << iterations << " итераций)..." << std::endl;

        clientCount.store(0, std::memory_order_release);
        isRunning.store(true, std::memory_order_release);

        std::thread client([this, iterations]() {
            for (int i = 0; i < iterations; ++i) {
                clientCount.fetch_add(1, std::memory_order_release);
            }
            });

        std::thread oper([this, iterations]() {
            for (int i = 0; i < iterations; ++i) {
                clientCount.fetch_sub(1, std::memory_order_release);
            }
            });

        client.join();
        oper.join();

        int finalCount = clientCount.load(std::memory_order_acquire);
        std::cout << "Стресс-тест завершен. Ожидалось: 0, Получено: " << finalCount;
        if (finalCount == 0) {
            std::cout << " ✓" << std::endl;
        }
        else {
            std::cout << " ✗" << std::endl;
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    const int MAX_CLIENTS = 5;
    const int SIMULATION_TIME = 10;
    const int STRESS_TEST_ITERATIONS = 100000;

    std::cout << "=== СИМУЛЯЦИЯ ОЧЕРЕДИ С АТОМАРНЫМИ ОПЕРАЦИЯМИ ===" << std::endl;
    std::cout << "Максимальное количество клиентов: " << MAX_CLIENTS << std::endl;

    // Тестирование различных моделей памяти
    for (int mode = 0; mode < 3; ++mode) {
        AtomicQueueSimulator simulator(MAX_CLIENTS);

        std::string modeName;
        switch (mode) {
        case 0: modeName = "SEQ_CST"; break;
        case 1: modeName = "RELEASE/ACQUIRE"; break;
        case 2: modeName = "RELAXED"; break;
        }

        std::cout << "\n--- Режим: " << modeName << " ---" << std::endl;

        std::thread clientThread;
        switch (mode) {
        case 0:
            clientThread = std::thread(&AtomicQueueSimulator::clientThreadSeqCst, &simulator);
            break;
        case 1:
            clientThread = std::thread(&AtomicQueueSimulator::clientThreadReleaseAcquire, &simulator);
            break;
        case 2:
            clientThread = std::thread(&AtomicQueueSimulator::clientThreadRelaxed, &simulator);
            break;
        }

        std::thread operatorThread(&AtomicQueueSimulator::operatorThread, &simulator, mode);

        std::this_thread::sleep_for(std::chrono::seconds(SIMULATION_TIME));

        simulator.stop();
        clientThread.join();
        operatorThread.join();

        std::cout << "Режим " << modeName << " завершен. Осталось клиентов: "
            << simulator.getClientCount() << std::endl;
    }

    // Стресс-тест для проверки корректности
    std::cout << "\n=== СТРЕСС-ТЕСТ КОРРЕКТНОСТИ ===" << std::endl;
    AtomicQueueSimulator tester(MAX_CLIENTS);
    tester.stressTest(STRESS_TEST_ITERATIONS);

    // Демонстрация работы с barrier для упорядочения
    std::cout << "\n=== ДЕМОНСТРАЦИЯ БАРЬЕРОВ ПАМЯТИ ===" << std::endl;
    {
        std::atomic<int> data{ 0 };
        std::atomic<bool> ready{ false };

        std::thread writer([&]() {
            data.store(42, std::memory_order_relaxed);
            ready.store(true, std::memory_order_release); // Release barrier
            });

        std::thread reader([&]() {
            while (!ready.load(std::memory_order_acquire)) {} // Acquire barrier
            int value = data.load(std::memory_order_relaxed);
            std::cout << "Прочитано значение: " << value << " (корректно благодаря барьерам)" << std::endl;
            });

        writer.join();
        reader.join();
    }

    return 0;
}