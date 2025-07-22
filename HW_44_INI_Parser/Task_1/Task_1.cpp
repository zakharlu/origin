#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <cstdlib> // Для strtol, strtod
#include <cctype>  // Для isspace

class ini_parser {
private:
    std::map<std::string, std::map<std::string, std::string>> data;

    // Функция для удаления пробелов в начале и конце строки
    static std::string trim(const std::string& s) {
        auto start = s.begin();
        while (start != s.end() && std::isspace(static_cast<unsigned char>(*start))) {
            ++start;
        }

        auto end = s.end();
        while (end != start && std::isspace(static_cast<unsigned char>(*(end - 1)))) {
            --end;
        }

        return std::string(start, end);
    }

    // Парсинг строки INI-файла
    void parse_line(const std::string& line, size_t line_num, std::string& current_section) {
        std::string trimmed = trim(line);

        // Пропускаем пустые строки и комментарии
        if (trimmed.empty() || trimmed[0] == ';') {
            return;
        }

        // Обработка секции
        if (trimmed[0] == '[' && trimmed.back() == ']') {
            current_section = trim(trimmed.substr(1, trimmed.size() - 2));
            if (current_section.empty()) {
                throw std::runtime_error("Empty section name at line " + std::to_string(line_num));
            }
            return;
        }

        // Обработка переменных
        size_t equal_pos = trimmed.find('=');
        if (equal_pos == std::string::npos) {
            throw std::runtime_error("Invalid syntax at line " + std::to_string(line_num));
        }

        std::string key = trim(trimmed.substr(0, equal_pos));
        std::string value = trim(trimmed.substr(equal_pos + 1));

        if (key.empty()) {
            throw std::runtime_error("Empty key at line " + std::to_string(line_num));
        }

        // Удаляем комментарий в конце значения
        size_t comment_pos = value.find(';');
        if (comment_pos != std::string::npos) {
            value = trim(value.substr(0, comment_pos));
        }

        if (current_section.empty()) {
            throw std::runtime_error("Variable outside section at line " + std::to_string(line_num));
        }

        data[current_section][key] = value;
    }

public:
    explicit ini_parser(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }

        std::string line;
        std::string current_section;
        size_t line_num = 0;

        while (std::getline(file, line)) {
            ++line_num;
            try {
                parse_line(line, line_num, current_section);
            }
            catch (const std::exception& e) {
                file.close();
                throw std::runtime_error("Error in " + filename + " at line " +
                    std::to_string(line_num) + ": " + e.what());
            }
        }
        file.close();
    }

    // Шаблонная функция для получения значения
    template<typename T>
    T get_value(const std::string& key) const {
        size_t dot_pos = key.find('.');
        if (dot_pos == std::string::npos) {
            throw std::runtime_error("Invalid key format. Use 'section.key'");
        }

        std::string section = key.substr(0, dot_pos);
        std::string variable = key.substr(dot_pos + 1);

        auto section_it = data.find(section);
        if (section_it == data.end()) {
            std::string available_sections;
            for (const auto& sec : data) {
                if (!available_sections.empty()) available_sections += ", ";
                available_sections += sec.first;
            }
            throw std::runtime_error("Section '" + section + "' not found. Available sections: " +
                (available_sections.empty() ? "none" : available_sections));
        }

        auto var_it = section_it->second.find(variable);
        if (var_it == section_it->second.end()) {
            std::string available_vars;
            for (const auto& var : section_it->second) {
                if (!available_vars.empty()) available_vars += ", ";
                available_vars += var.first;
            }
            throw std::runtime_error("Variable '" + variable + "' not found in section '" + section +
                "'. Available variables: " + (available_vars.empty() ? "none" : available_vars));
        }

        const std::string& value = var_it->second;

        try {
            if constexpr (std::is_same_v<T, std::string>) {
                return value;
            }
            else if constexpr (std::is_integral_v<T>) {
                // Преобразование для целочисленных типов
                char* end_ptr;
                long result = std::strtol(value.c_str(), &end_ptr, 10);

                // Проверка на ошибки преобразования
                if (end_ptr == value.c_str() || *end_ptr != '\0') {
                    throw std::runtime_error("'" + value + "' is not a valid integer");
                }

                // Проверка на переполнение
                if (result < std::numeric_limits<T>::min() || result > std::numeric_limits<T>::max()) {
                    throw std::runtime_error("Integer overflow for value '" + value + "'");
                }

                return static_cast<T>(result);
            }
            else if constexpr (std::is_floating_point_v<T>) {
                // Преобразование для дробных чисел
                char* end_ptr;
                double result = std::strtod(value.c_str(), &end_ptr);

                // Проверка на ошибки преобразования
                if (end_ptr == value.c_str() || *end_ptr != '\0') {
                    throw std::runtime_error("'" + value + "' is not a valid number");
                }

                return static_cast<T>(result);
            }
            else {
                static_assert(std::is_same_v<T, T>, "Unsupported type requested");
            }
        }
        catch (const std::exception& e) {
            throw std::runtime_error("Failed to convert value '" + value + "' to requested type: " + e.what());
        }
    }
};


int main() {
    try {
        ini_parser parser("example.ini");

        double name1 = parser.get_value<double>("Section1.var1");
        std::cout << "Found value:  " << name1 << std::endl;
        
        std::string name2 = parser.get_value<std::string>("Section1.var2");
        std::cout << "Found value: " << name2 << std::endl;

        std::string name3 = parser.get_value<std::string>("Section1.var3");
        std::cout << "Found value: " << name3 << std::endl;

        std::string name4 = parser.get_value<std::string>("Section4.Mode");
        std::cout << "Found value: " << name4 << std::endl;
        
        std::string name5 = parser.get_value<std::string>("Section1.var4");
        std::cout << "Found value: " << name5 << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}