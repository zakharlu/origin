#include <iostream>
#include <string>
#include <cmath>

unsigned long long real_string_hash(const std::string& str, unsigned int p, unsigned int n) {
    unsigned long long sum = 0;

    for (size_t i = 0; i < str.size(); ++i) {
        unsigned long long power = 1;
        for (size_t j = 0; j < i; ++j) {
            power *= p;
        }
        sum += static_cast<unsigned long long>(str[i]) * power;
    }

    return sum % n;
}

int main() {
    setlocale(LC_ALL, "Russian");
    unsigned int p, n;
    std::cout << "Введите p: ";
    std::cin >> p;
    std::cout << "Введите n: ";
    std::cin >> n;

    std::string input;
    do {
        std::cout << "Введите строку: ";
        std::cin >> input;
        unsigned long long hash = real_string_hash(input, p, n);
        std::cout << "Хэш строки " << input << " = " << hash << std::endl;
    } while (input != "exit");

    return 0;
} 