#include <iostream>
#include <vector>
#include <algorithm> // для std::for_each

class Counter {
private:
    int sum = 0;
    int count = 0;

public:
    // Оператор вызова функции
    void operator()(int num) {
        if (num % 3 == 0) {
            sum += num;
            count++;
        }
    }

    // Метод для получения суммы
    int get_sum() const {
        return sum;
    }

    // Метод для получения количества
    int get_count() const {
        return count;
    }
};

int main() {
    std::vector<int> numbers = { 4, 1, 3, 6, 25, 54 };

    Counter counter;

    // Применяем функтор к каждому элементу вектора
    std::for_each(numbers.begin(), numbers.end(), std::ref(counter));

    // Выводим результаты
    std::cout << "get_sum() = " << counter.get_sum() << std::endl;
    std::cout << "get_count() = " << counter.get_count() << std::endl;

    return 0;
}