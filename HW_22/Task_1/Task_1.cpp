#include <iostream>
#include <string>

class BadLength : public std::exception
{
public:
    const char* what() const override { return "Вы ввели слово запретной длины! До свидания"; }
};

int function(std::string str, int forbidden_length);

int main() {
    int len_str = 0, good_str = 0;
    std::string str = "";

    setlocale(LC_ALL, "Russian");
    std::cout << "Введите запретную длину: ";
    std::cin >> len_str;
    while (true) {
        std::cout << "Введите слово: ";
        std::cin >> str;
        try {
            good_str = function(str, len_str);
            std::cout << "Длина слова \"" << str << "\" равна " << good_str << std::endl;
        }
        catch (const BadLength bad_length) {
            std::cout << bad_length.what() << std::endl;
            break;
        }
    }

    return 0;
}
int function(std::string str, int forbidden_length) {
    if (str.length() == forbidden_length) throw BadLength();
    return str.length();
}