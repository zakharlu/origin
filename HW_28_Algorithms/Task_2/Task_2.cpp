// Task_2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

int find_cnt(int* arr, int size, int point);
int main()
{
    setlocale(LC_ALL, "Russian");
    int arr[] = {14, 16, 19, 32, 32, 32, 56, 69, 72};
    int point = 0, cnt = 0;
    int size = sizeof(arr) / sizeof(arr[0]);
    std::cout << "Введите точку отсчета: ";
    std::cin >> point;
    cnt = find_cnt(arr, size, point);
    std::cout << "Количество элементов в массиве больших, чем " << point << ": " << cnt << std::endl;
  
}

int find_cnt(int* arr, int size, int point) {
    int left = 0, right = size - 1;
    while (right - left > 1) {
        int middle = left + (right - left) / 2;
        if (arr[middle] > point) {
            right = middle;
        }
        else {
            left = middle;
        }
    }
    if (right == (size - 1) && arr[right] <= point) {
        right++;
    }
    if (left == 0 && arr[left] > point) {
        right--;
    }
    return size - right;
}