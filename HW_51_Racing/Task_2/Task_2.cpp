#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <string>
#include <sstream>

class ProgressBar {
private:
    std::mutex console_mutex;
    std::vector<std::string> progress_lines;
    int thread_count;

    std::string get_time_string(long long milliseconds) {
        long long minutes = milliseconds / 60000;
        long long seconds = (milliseconds % 60000) / 1000;
        long long ms = milliseconds % 1000;

        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << minutes << ":"
            << std::setw(2) << std::setfill('0') << seconds << "."
            << std::setw(3) << std::setfill('0') << ms;
        return ss.str();
    }

    void update_display() {
        // Очищаем область вывода прогресса и выводим все строки
        std::cout << "\033[7;1H"; // Перемещаемся к началу прогресса

        for (const auto& line : progress_lines) {
            std::cout << line << "\033[K" << std::endl; // Выводим строку и очищаем остаток
        }
        std::cout.flush();
    }

public:
    void run_simulation(int thread_count, int calculation_length, int delay_per_step_ms) {
        this->thread_count = thread_count;
        progress_lines.resize(thread_count);

        std::cout << "Многопоточный расчёт с прогресс-баром" << std::endl;
        std::cout << "=====================================" << std::endl << std::endl;

        std::cout << "Количество потоков: " << thread_count << std::endl;
        std::cout << "Длина расчёта: " << calculation_length << " шагов" << std::endl;
        std::cout << "Задержка на шаг: " << delay_per_step_ms << " мс" << std::endl << std::endl;

        // Инициализируем начальные строки
        std::cout << "Прогресс потоков:" << std::endl;
        for (int i = 0; i < thread_count; i++) {
            progress_lines[i] = "Поток " + std::to_string(i + 1) + " [                              ]   0% Время: 00:00.000";
        }
        update_display();
        std::cout << std::endl;

        std::vector<std::thread> threads;
        for (int i = 0; i < thread_count; i++) {
            threads.emplace_back(&ProgressBar::calculate, this, i + 1, calculation_length, delay_per_step_ms);
        }

        for (auto& thread : threads) {
            thread.join();
        }

        std::cout << "\nВсе потоки завершили работу!" << std::endl;
    }

private:
    void calculate(int thread_number, int calculation_length, int delay_per_step_ms) {
        auto start = std::chrono::steady_clock::now();
        int index = thread_number - 1;

        try {
            for (int i = 0; i <= calculation_length; i++) {
                {
                    std::lock_guard<std::mutex> lock(console_mutex);

                    int bars = (i * 30) / std::max(1, calculation_length);
                    int percent = (i * 100) / std::max(1, calculation_length);

                    auto now = std::chrono::steady_clock::now();
                    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);

                    // Формируем строку прогресса
                    std::stringstream ss;
                    ss << "Поток " << thread_number << " [";
                    ss << std::string(bars, '=');
                    ss << std::string(30 - bars, ' ');
                    ss << "] " << std::setw(3) << percent << "%";
                    ss << " Время: " << get_time_string(elapsed.count());

                    progress_lines[index] = ss.str();
                    update_display();
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(delay_per_step_ms));
            }

            {
                std::lock_guard<std::mutex> lock(console_mutex);
                auto end = std::chrono::steady_clock::now();
                auto total_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

                std::stringstream ss;
                ss << "Поток " << thread_number << " [==============================] 100%";
                ss << " Завершено! Время: " << get_time_string(total_elapsed.count());

                progress_lines[index] = ss.str();
                update_display();
            }

        }
        catch (...) {
            std::lock_guard<std::mutex> lock(console_mutex);
            progress_lines[index] = "Поток " + std::to_string(thread_number) + " ошибка выполнения";
            update_display();
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    ProgressBar pb;
    pb.run_simulation(5, 50, 80);
    return 0;
}
