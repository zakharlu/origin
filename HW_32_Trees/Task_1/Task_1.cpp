#include <iostream>
#include <vector>
#include <string>

void print_pyramid(const std::vector<int>& arr) {
    std::cout << "Пирамида:" << std::endl;
    for (int i = 0; i < arr.size(); ++i) {
        if (i == 0) {
            std::cout << "0 root " << arr[i] << "\n";
        }
        else {
            int parent_index = (i - 1) / 2;
            int level = 0;
            int temp = i;
            while (temp > 0) {
                temp = (temp - 1) / 2;
                ++level;
            }
            if (i % 2 == 1) {
                std::cout << level << " left(" << arr[parent_index] << ") " << arr[i] << "\n";
            }
            else {
                std::cout << level << " right(" << arr[parent_index] << ") " << arr[i] << "\n";
            }
        }
    }
}

void input_array(std::vector<int>& arr) {
    std::cout << "Введите элементы массива через пробел и нажмите Enter: ";

    std::string input_line;
    std::getline(std::cin, input_line);

    int num = 0;
    bool negative = false;
    bool reading_number = false;

    for (char c : input_line) {
        if (c == ' ') {
            if (reading_number) {
                arr.push_back(negative ? -num : num);
                num = 0;
                negative = false;
                reading_number = false;
            }
        }
        else if (c == '-') {
            negative = true;
        }
        else if (c >= '0' && c <= '9') {
            num = num * 10 + (c - '0');
            reading_number = true;
        }
    }

    // Добавляем последнее число, если оно есть
    if (reading_number) {
        arr.push_back(negative ? -num : num);
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::vector<int> arr;

    input_array(arr);

    std::cout << "Исходный массив: ";
    for (int i = 0; i < arr.size(); ++i) {
        std::cout << arr[i];
        if (i != arr.size() - 1) {
            std::cout << " ";
        }
    }
    std::cout << "\n";

    print_pyramid(arr);

    return 0;
}