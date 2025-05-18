// Task_2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

void quick_sort(int* arr, int size);

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

    quick_sort(arr, size);

    std::cout << "Отсортированный массив: ";
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
}


void quick_sort(int* arr, int size) {
    if (arr == nullptr || size <= 1) {
        return;
    }

    int* stack = new int[size];
    int top = -1;

    // Изначально обрабатываем весь массив
    stack[++top] = 0;
    stack[++top] = size - 1;

    while (top >= 0) {
        // Извлекаем границы текущего подмассива
        int high = stack[top--];
        int low = stack[top--];

        // Выбираем опорный элемент (последний элемент подмассива)
        int pivot = arr[high];
        int i = low - 1;

        // Перераспределяем элементы относительно опорного
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                // Меняем местами arr[i] и arr[j]
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        // Ставим опорный элемент на правильное место
        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        int pivot_idx = i + 1;

        // Если есть элементы слева от опорного, добавляем в стек
        if (pivot_idx - 1 > low) {
            stack[++top] = low;
            stack[++top] = pivot_idx - 1;
        }

        // Если есть элементы справа от опорного, добавляем в стек
        if (pivot_idx + 1 < high) {
            stack[++top] = pivot_idx + 1;
            stack[++top] = high;
        }
    }

    delete[] stack;
}