#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <iomanip>
#include <algorithm>

class VectorSumCalculator {
private:
    std::vector<int> vector1;
    std::vector<int> vector2;
    std::vector<int> result;
    size_t size;

    // Функция для выполнения сложения в определенном диапазоне
    void sumRange(size_t start, size_t end) {
        for (size_t i = start; i < end; ++i) {
            result[i] = vector1[i] + vector2[i];
        }
    }

public:
    VectorSumCalculator(size_t n) : size(n) {
        // Инициализация векторов случайными значениями
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 100);

        vector1.resize(size);
        vector2.resize(size);
        result.resize(size);

        for (size_t i = 0; i < size; ++i) {
            vector1[i] = dis(gen);
            vector2[i] = dis(gen);
        }
    }

    // Последовательное вычисление (1 поток)
    double calculateSequential() {
        auto start = std::chrono::high_resolution_clock::now();
        sumRange(0, size);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> duration = end - start;
        return duration.count();
    }

    // Параллельное вычисление с заданным количеством потоков
    double calculateParallel(int numThreads) {
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        size_t chunkSize = size / numThreads;

        for (int i = 0; i < numThreads; ++i) {
            size_t startIdx = i * chunkSize;
            size_t endIdx = (i == numThreads - 1) ? size : startIdx + chunkSize;

            threads.emplace_back(&VectorSumCalculator::sumRange, this, startIdx, endIdx);
        }

        for (auto& thread : threads) {
            thread.join();
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        return duration.count();
    }

    // Проверка корректности результата (для отладки)
    bool verifyResult() {
        for (size_t i = 0; i < size; ++i) {
            if (result[i] != vector1[i] + vector2[i]) {
                return false;
            }
        }
        return true;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    // Вывод количества аппаратных ядер
    unsigned int hardwareThreads = std::thread::hardware_concurrency();
    std::cout << "Количество аппаратных ядер - " << hardwareThreads << "\n\n";

    // Размеры массивов для тестирования
    std::vector<size_t> sizes = { 1000, 10000, 100000, 1000000 };
    std::vector<int> threadCounts = { 1, 2, 4, 8, 16 };

    // Создание таблицы для результатов
    std::vector<std::vector<double>> results(
        threadCounts.size(),
        std::vector<double>(sizes.size(), 0.0)
    );

    // Заполнение заголовков таблицы
    std::cout << "                  ";
    for (size_t size : sizes) {
        std::cout << size  << "       ";
    }
    std::cout << "\n";


    // Выполнение вычислений для каждого размера и количества потоков
    for (int i = 0; i < threadCounts.size(); ++i) {
        int threads = threadCounts[i];
        std::cout << std::setw(3) << threads << " " << std::setw(5) << "потоков";

        for (int j = 0; j < sizes.size(); ++j) {
            size_t size = sizes[j];

            VectorSumCalculator calculator(size);
            double duration;

            if (threads == 1) {
                duration = calculator.calculateSequential();
            }
            else {
                duration = calculator.calculateParallel(threads);
            }

            results[i][j] = duration;

            // Вывод времени с точностью до 7 знаков после запятой
            std::cout << std::setw(10) << std::fixed << std::setprecision(7)
                << duration << "s |";
        }
        std::cout << "\n";
    }

    // Анализ результатов - определение оптимального количества потоков
    std::cout << "\nАнализ результатов:\n";
    for (int j = 0; j < sizes.size(); ++j) {
        double minTime = results[0][j];
        int bestThreads = 1;

        for (int i = 1; i < threadCounts.size(); ++i) {
            if (results[i][j] < minTime) {
                minTime = results[i][j];
                bestThreads = threadCounts[i];
            }
        }

        std::cout << "Для " << sizes[j] << " элементов: лучший результат с "
            << bestThreads << " потоками (" << minTime << "s)\n";
    }

    return 0;
}