#include <iostream>
#include <vector>
#include <type_traits>

// Возведение в квадрат простых чисел
template<typename T>
T square(T x) {
    return x * x;
}

// Возведение в квадрат каждого элемента вектора
template<typename T>
std::vector<T> square(std::vector<T>& vec) {
    std::vector<T> result;
    for (auto& elem: vec) {
        result.push_back(elem * elem);
    }
    return result;
}

// Вывод вектора
template<typename T>
void print_vector(std::vector<T>& vec) {
    bool first = true;
    for (auto elem : vec) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << elem;
        first = false;
    }
    std::cout << std::endl;
}

int main() {
    int num = 4;
    std::cout << "[IN]: " << num << std::endl;
    std::cout << "[OUT]: " << square(num) << std::endl;

    std::vector<int> vec = { -1, 4, 8 };
    std::cout << "[IN]: ";
    print_vector(vec);
    auto squared_vec = square(vec);
    std::cout << "[OUT]: ";
    print_vector(squared_vec); 

    return 0;
}