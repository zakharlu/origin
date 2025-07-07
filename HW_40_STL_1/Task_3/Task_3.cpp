#include <iostream>
#include <stdexcept> // для std::out_of_range

template <typename T>
class SimpleVector {
private:
    T* data;         // Указатель на массив элементов
    size_t vec_size;     // Текущее количество элементов
    size_t vec_capacity; // Текущая вместимость

    // Увеличение вместимости
    void reallocate(size_t new_capacity) {
        T* new_data = new T[new_capacity];

        // Копируем существующие элементы
        for (size_t i = 0; i < vec_size; ++i) {
            new_data[i] = data[i];
        }

        delete[] data;
        data = new_data;
        vec_capacity = new_capacity;
    }

public:
    // Конструктор по умолчанию
    SimpleVector() : data(nullptr), vec_size(0), vec_capacity(0) {}

    // Деструктор
    ~SimpleVector() {
        delete[] data;
    }

    // Доступ к элементу, +проверяем границы
    T& at(size_t index) {
        if (index >= vec_size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // Добавление элемента в конец
    void push_back(T value) {
        if (vec_size >= vec_capacity) {
            // Увеличиваем вместимость (в 2 раза или до 1, если capacity = 0)
            size_t new_capacity = vec_capacity == 0 ? 1 : vec_capacity * 2;
            reallocate(new_capacity);
        }
        data[vec_size++] = value;
    }

    // Текущее количество элементов
    size_t size() const {
        return vec_size;
    }

    // Текущая вместимость
    size_t capacity() const {
        return vec_capacity;
    }
};

int main() {
    SimpleVector<int> vec;

    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    std::cout << "Size: " << vec.size() << "\n";
    std::cout << "Capacity: " << vec.capacity() << "\n";

    try {
        std::cout << "At 1: " << vec.at(1) << "\n";
        std::cout << "At 5: " << vec.at(5) << "\n"; // Должен Выбросить исключение
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}