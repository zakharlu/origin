#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>

class AtomicQueueSimulator {
private:
    std::atomic<int> clientCount;
    int maxClients;
    std::atomic<bool> isRunning;
    std::mutex coutMutex; // для безопасного вывода

public:
    AtomicQueueSimulator(int maxClients) : clientCount(0), maxClients(maxClients), isRunning(true) {}

    void clientThread() {
        while (isRunning) {
            std::this_thread::sleep_for(std::chrono::seconds(1));

            if (clientCount < maxClients) {
                clientCount++;
                std::lock_guard<std::mutex> lock(coutMutex);
                std::cout << "Клиент пришел. В очереди: " << clientCount << " клиентов" << std::endl;
            }
            else {
                std::lock_guard<std::mutex> lock(coutMutex);
                std::cout << "Клиент пришел, но очередь полна! (" << clientCount << " клиентов)" << std::endl;
            }
        }
    }

    void operatorThread() {
        while (isRunning || clientCount > 0) {
            std::this_thread::sleep_for(std::chrono::seconds(2));

            if (clientCount > 0) {
                clientCount--;
                std::lock_guard<std::mutex> lock(coutMutex);
                std::cout << "Операционист обслужил клиента. В очереди: " << clientCount << " клиентов" << std::endl;
            }
        }
    }

    void stop() {
        isRunning = false;
    }

    int getClientCount() {
        return clientCount.load();
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    const int MAX_CLIENTS = 5;
    const int SIMULATION_TIME = 12;

    AtomicQueueSimulator simulator(MAX_CLIENTS);

    std::cout << "Запуск симуляции с atomic переменными" << std::endl;
    std::cout << "Максимальное количество клиентов: " << MAX_CLIENTS << std::endl;

    std::thread clientThread(&AtomicQueueSimulator::clientThread, &simulator);
    std::thread operatorThread(&AtomicQueueSimulator::operatorThread, &simulator);

    std::this_thread::sleep_for(std::chrono::seconds(SIMULATION_TIME));

    simulator.stop();
    clientThread.join();
    operatorThread.join();

    std::cout << "Симуляция завершена. Осталось клиентов: " << simulator.getClientCount() << std::endl;

    return 0;
}