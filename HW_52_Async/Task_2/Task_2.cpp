#include <iostream>
#include <vector>
#include <future>
#include <algorithm>
#include <iterator>

// Параллельный for_each с рекурсивным разделением
template<typename Iterator, typename Func>
void parallel_for_each(Iterator first, Iterator last, Func func,
    size_t min_block_size = 1000) {
    // Вычисляем длину диапазона
    auto length = std::distance(first, last);

    // Если диапазон слишком мал для разделения, обрабатываем последовательно
    if (length <= min_block_size) {
        std::for_each(first, last, func);
        return;
    }

    // Находим середину диапазона
    Iterator mid = first;
    std::advance(mid, length / 2);

    // Рекурсивно запускаем обработку левой и правой частей асинхронно
    auto left_future = std::async(std::launch::async,
        [first, mid, &func, min_block_size]() {
            parallel_for_each(first, mid, func, min_block_size);
        });

    // Правую часть обрабатываем рекурсивно в текущем потоке
    parallel_for_each(mid, last, func, min_block_size);

    // Ожидаем завершения левой части
    left_future.get();
}

// Демонстрация
int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "Параллельный for_each с рекурсивным разделением" << std::endl;
    std::cout << "==============================================" << std::endl;

    // Демонстрация с контейнером
    std::vector<int> data = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    std::cout << "До обработки: ";
    for (int val : data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // Умножаем каждый элемент на 2
    parallel_for_each(data.begin(), data.end(), [](int& val) {
        val *= 2;
        }, 3);

    std::cout << "После обработки: ";
    for (int val : data) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
