#include <iostream>
#include <stdexcept> // Для std::out_of_range

class smart_array {
private:
    int* Arr;
    int size;
    int capacity;

public:
    smart_array(int init_capacity) {
        Arr = new int[init_capacity];
        size = 0;
        capacity = init_capacity;
    }

    // Оператор присваивания
    smart_array& operator=(const smart_array& other) {
        if (this == &other) { // Проверка на самоприсваивание
            return *this;
        }

        // Освобождаем старую память
        delete[] Arr;

        // Копируем размер и вместимость
        size = other.size;
        capacity = other.capacity;

        // Выделяем новую память и копируем элементы
        Arr = new int[capacity];
        for (int i = 0; i < size; ++i) {
            Arr[i] = other.Arr[i];
        }

        return *this;
    }

    void add_element(int value) {
        if (size >= capacity) {
            int new_capacity = capacity * 2;
            int* new_Arr = new int[new_capacity];
            for (int i = 0; i < capacity; ++i) {
                new_Arr[i] = Arr[i];
            }
            delete[] Arr;
            Arr = new_Arr;
            capacity = new_capacity;
        }

        Arr[size++] = value;
    }

    // Получение элемента по индексу
    int get_element(int index) const {
        if (index >= size) {
            throw std::out_of_range("Индекс больше размера массива");
        }
        return Arr[index];
    }

    // Получение текущего размера
    int get_size() const {
        return size;
    }

    // Получение текущей вместимости
    int get_capacity() const {
        return capacity;
    }

    ~smart_array() {
        delete[] Arr;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    try {
        smart_array arr(5);
        arr.add_element(1);
        arr.add_element(4);
        arr.add_element(155);

        smart_array new_array(2);
        new_array.add_element(44);
        new_array.add_element(34);

        arr = new_array; 


        for (int i = 0; i < arr.get_size(); ++i) {
            std::cout << "Элемент " << i << ": " << arr.get_element(i) << std::endl;
        }
    }
    catch (const std::exception& ex) {
        std::cout << "Ошибка: " << ex.what() << std::endl;
    }

    return 0;
}