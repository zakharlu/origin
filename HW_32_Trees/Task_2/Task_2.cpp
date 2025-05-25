#include <iostream>
#include <vector>
#include <string>

void print_pyramid(const std::vector<int>& pyramid) {
    std::cout << "Пирамида:" << std::endl;
    for (int i = 0; i < pyramid.size(); ++i) {
        if (i == 0) {
            std::cout << "0 root " << pyramid[i] << std::endl;
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
                std::cout << level << " left(" << pyramid[parent_index] << ") " << pyramid[i] << std::endl;
            }
            else {
                std::cout << level << " right(" << pyramid[parent_index] << ") " << pyramid[i] << std::endl;
            }
        }
    }
}

bool get_parent_index(int current, int& parent) {
    if (current == 0) return false;
    parent = (current - 1) / 2;
    return true;
}

bool get_left_child(int current, int& left, int size) {
    left = 2 * current + 1;
    return left < size;
}

bool get_right_child(int current, int& right, int size) {
    right = 2 * current + 2;
    return right < size;
}

std::string get_node_info(const std::vector<int>& pyramid, int index) {
    if (index == 0) {
        return "0 root " + std::to_string(pyramid[index]);
    }

    int parent_index = (index - 1) / 2;
    int level = 0;
    int temp = index;
    while (temp > 0) {
        temp = (temp - 1) / 2;
        ++level;
    }

    if (index % 2 == 1) {
        return std::to_string(level) + " left(" + std::to_string(pyramid[parent_index]) + ") " + std::to_string(pyramid[index]);
    }
    else {
        return std::to_string(level) + " right(" + std::to_string(pyramid[parent_index]) + ") " + std::to_string(pyramid[index]);
    }
}

void pyramid_travel(const std::vector<int>& pyramid) {
    int current = 0;
    std::string command;

    do {
        std::cout << "Вы находитесь здесь: " << get_node_info(pyramid, current) << std::endl;
        std::cout << "Введите команду: ";
        std::cin >> command;

        if (command == "up") {
            int parent;
            if (get_parent_index(current, parent)) {
                current = parent;
                std::cout << "Ок" << std::endl;
            }
            else {
                std::cout << "Ошибка! Отсутствует родитель" << std::endl;
            }
        }
        else if (command == "left") {
            int left;
            if (get_left_child(current, left, pyramid.size())) {
                current = left;
                std::cout << "Ок" << std::endl;
            }
            else {
                std::cout << "Ошибка! Отсутствует левый потомок" << std::endl;
            }
        }
        else if (command == "right") {
            int right;
            if (get_right_child(current, right, pyramid.size())) {
                current = right;
                std::cout << "Ок" << std::endl;
            }
            else {
                std::cout << "Ошибка! Отсутствует правый потомок" << std::endl;
            }
        }
        else if (command == "exit") {
            break;
        }
        else {
            std::cout << "Неизвестная команда" << std::endl;
        }

    } while (true);
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::vector<std::vector<int>> test_pyramids = {
        {1, 3, 6, 5, 9, 8},
        {94, 67, 18, 44, 55, 12, 6, 42},
        {16, 11, 9, 10, 5, 6, 8, 1, 2, 4}
    };

    for (const auto& pyramid : test_pyramids) {
        std::cout << "Исходный массив: ";
        for (int num : pyramid) {
            std::cout << num << " ";
        }
        std::cout << std::endl;

        print_pyramid(pyramid);
        pyramid_travel(pyramid);

        std::cout << std::endl;
    }

    return 0;
}