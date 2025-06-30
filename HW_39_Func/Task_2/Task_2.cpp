#include <iostream>
#include <vector>

template <typename T>
class table {
private:
    // Внутреннее хранилище данных - вектор векторов
    std::vector<std::vector<T>> data;

public:
    // Конструктор, создающий таблицу  с заданными размерами таблицы
    // Конструктор создает вектор из rows элементов, каждый элемент - вектор из cols элементов 
    table(int rows, int cols) : data(rows, std::vector<T>(cols)) {}

    // Константный доступ к строкам для чтения
    const std::vector<T>& operator[](int row) const {
        return data[row];
    }

    // Неконстантный доступ к строкам для модификации элементов
    std::vector<T>& operator[](int row) {
        return data[row];
    }


    // Получение размеров таблицы
    std::pair<int, int> Size() const {
        if (data.empty()) return { 0, 0 };
        return { data.size(), data[0].size() };
    }
};

int main() {
    auto test = table<int>(2, 3);  // Создаем таблицу 2x3
    test[0][0] = 4;                 // Записываем значение
    std::cout << test[0][0];       // Выводит 4

    return 0;
}