#include <iostream>
#include <mutex>
#include <utility>

// Класс Data с скалярными данными и мьютексом
class Data {
private:
    int value;
    std::mutex mtx;

public:
    // Конструктор
    Data(int val = 0) : value(val) {}

    // Метод для получения значения 
    int getValue() const {
        return value;
    }

    // Метод для установки значения 
    void setValue(int val) {
        std::lock_guard<std::mutex> lock(mtx);
        value = val;
    }

    // Дружественные функции для доступа к приватным членам
    friend void swap_using_lock(Data& a, Data& b);
    friend void swap_using_scoped_lock(Data& a, Data& b);
    friend void swap_using_unique_lock(Data& a, Data& b);

    // Метод для вывода информации об объекте
    void print(const std::string& name) const {
        std::cout << name << ": value = " << value << std::endl;
    }
};

// Вариант 1: Использование std::lock с lock_guard
void swap_using_lock(Data& a, Data& b) {
    // Захватываем оба мьютекса одновременно
    std::lock(a.mtx, b.mtx);

    // Используем lock_guard с adopt_lock для управления уже захваченными мьютексами
    std::lock_guard<std::mutex> lock_a(a.mtx, std::adopt_lock);
    std::lock_guard<std::mutex> lock_b(b.mtx, std::adopt_lock);

    // Выполняем обмен данными
    std::swap(a.value, b.value);

}

// Вариант 2: Использование std::scoped_lock (C++17)
void swap_using_scoped_lock(Data& a, Data& b) {
    // scoped_lock автоматически захватывает все переданные мьютексы безопасным способом
    std::scoped_lock lock(a.mtx, b.mtx);

    // Выполняем обмен данными
    std::swap(a.value, b.value);

}

// Вариант 3: Использование std::unique_lock
void swap_using_unique_lock(Data& a, Data& b) {
    // Создаем unique_lock для каждого мьютекса, но пока не захватываем их
    std::unique_lock<std::mutex> lock_a(a.mtx, std::defer_lock);
    std::unique_lock<std::mutex> lock_b(b.mtx, std::defer_lock);

    // Захватываем оба мьютекса одновременно
    std::lock(lock_a, lock_b);

    // Выполняем обмен данными
    std::swap(a.value, b.value);

    // unique_lock автоматически освободит мьютексы при выходе из области видимости
}


int main() {
    Data data1(10);
    Data data2(20);

    std::cout << "До обмена:" << std::endl;
    data1.print("data1");
    data2.print("data2");

    // Тестируем первый вариант
    swap_using_lock(data1, data2);
    std::cout << "\nПосле swap_using_lock:" << std::endl;
    data1.print("data1");
    data2.print("data2");

    // Тестируем второй вариант
    swap_using_scoped_lock(data1, data2);
    std::cout << "\nПосле swap_using_scoped_lock:" << std::endl;
    data1.print("data1");
    data2.print("data2");

    // Тестируем третий вариант
    swap_using_unique_lock(data1, data2);
    std::cout << "\nПосле swap_using_unique_lock:" << std::endl;
    data1.print("data1");
    data2.print("data2");

    return 0;
}