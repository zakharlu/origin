#include <iostream>
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
    int get_element(int index) {
        if (index >= size) {
            throw std::out_of_range("Индекс больше размера массива");
        }
        return Arr[index];
    }

    // Получение текущего размера
    int get_size() {
        return size;
    }

    // Получение текущей вместимости
    int get_capacity() {
        return capacity;
    }

    ~smart_array() {
        delete[] Arr;
    }
};


int main()
{
    setlocale(LC_ALL, "Russian");
    try {
        smart_array arr(5);
        arr.add_element(1);
        arr.add_element(4);
        arr.add_element(155);
        arr.add_element(14);
        arr.add_element(15);

        // Если добавить еще элементы, вызовется расширение массива
        arr.add_element(42);
        arr.add_element(99);

        for (int i = 0; i < arr.get_size(); ++i) {
            std::cout << "Элемент " << i << ": " << arr.get_element(i) << std::endl;
            std::cout << "Размер массива: " << arr.get_size() << std::endl;
            std::cout << "Емкость массива: " << arr.get_capacity() << std::endl;
            std::cout << std::endl;
        }

        // Попытка доступа к несуществующему элементу
        std::cout << arr.get_element(10) << std::endl;
    }
    catch (const std::exception& ex) {
        std::cout << "Ошибка: " << ex.what() << std::endl;
    }

    return 0;
}
