#include <iostream>
#include "../DinamycLib/DinamycLib/Leaver.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    Leaver leaver;
    std::string name;
    std::cout << "Введите имя: ";
    std::cin >> name;
    std::cout << leaver.leave(name) << std::endl;

    return 0;
}