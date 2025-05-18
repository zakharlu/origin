#include <iostream>

void count_sort(int* arr, int size);

int main() {
    int size;
    setlocale(LC_ALL, "Russian");

    std::cout << "Введите размер массива: ";
    std::cin >> size;


    int* arr = new int[size];


    std::cout << "Введите массив: ";
    for (int i = 0; i < size; i++) {
        std::cin >> arr[i];
    }

    std::cout << "Массив до сортировки: ";
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }

    std::cout << std::endl;

    count_sort(arr, size);

    std::cout << "Отсортированный массив :";
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
}

void count_sort(int* arr, int size) {
    const int min_value = 10;
    const int max_value = 24;
    const int range = max_value - min_value + 1;

    // Создаём массив для подсчёта частот
    int count[range] = { 0 };

    // Подсчитываем количество каждого элемента
    for (int i = 0; i < size; ++i) {
        if (arr[i] >= min_value && arr[i] <= max_value) {
            count[arr[i] - min_value]++;
        }
    }

    // Заполняем исходный массив отсортированными значениями
    int index = 0;
    for (int value = min_value; value <= max_value; ++value) {
        while (count[value - min_value] > 0) {
            arr[index++] = value;
            count[value - min_value]--;
        }
    }
}