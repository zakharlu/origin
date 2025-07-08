#include <vector>
#include <algorithm>

template<typename T>
void remove_duplicates(std::vector<T>& vec) {
    // Сортируем вектор
    std::sort(vec.begin(), vec.end());

    // Удаляем подряд идущие дубликаты
    auto last = std::unique(vec.begin(), vec.end());

    // Удаляем оставшиеся элементы
    vec.erase(last, vec.end());
}

#include <iostream>

int main() {
    std::vector<int> vec = {1, 1, 2, 5, 6, 1, 2, 4};

    remove_duplicates(vec);

    for (const auto& x : vec) {
        std::cout << x << " ";
    }

    return 0;
}