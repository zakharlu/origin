#include <iostream>
#include "Greeter.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    Greeter greeter;
    std::string name;
    std::cout << "Введите имя: ";
    std::cin >> name;
    std::cout << greeter.greet(name) << std::endl;

    return 0;
}