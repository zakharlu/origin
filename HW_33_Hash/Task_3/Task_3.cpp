#include <iostream>
#include <string>

int find_substring_light_rabin_karp(std::string source, std::string substring) {
    if (substring.empty()) {
        return -1;
    }

    int n = source.size();
    int m = substring.size();

    if (n < m) {
        return -1;
    }

    // {еш-функция: сумма кодов символов
    int substring_hash = 0;
    for (char c : substring) {
        substring_hash += c;
    }

    int window_hash = 0;
    for (int i = 0; i < m; ++i) {
        window_hash += source[i];
    }

    for (int i = 0; i <= n - m; ++i) {
        if (window_hash == substring_hash) {
            bool match = true;
            for (int j = 0; j < m; ++j) {
                if (source[i + j] != substring[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                return i;
            }
        }

        if (i < n - m) {
            window_hash = window_hash - source[i] + source[i + m];
        }
    }

    return -1;
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::string source;
    std::cout << "Введите строку, в которой будет осуществляться поиск: ";
    std::getline(std::cin, source);

    while (true) {
        std::string substring;
        std::cout << "Введите подстроку, которую нужно найти: ";
        std::getline(std::cin, substring);

        if (substring == "exit") {
            int index = find_substring_light_rabin_karp(source, substring);
            if (index != -1) {
                std::cout << "Подстрока " << substring << " найдена по индексу " << index << std::endl;
            }
            else {
                std::cout << "Подстрока " << substring << " не найдена" << std::endl;
            }
            break;
        }

        int index = find_substring_light_rabin_karp(source, substring);
        if (index != -1) {
            std::cout << "Подстрока " << substring << " найдена по индексу " << index << std::endl;
        }
        else {
            std::cout << "Подстрока " << substring << " не найдена" << std::endl;
        }
    }

    return 0;
}