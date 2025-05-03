#include <iostream>
#include <windows.h>
#include "Leaver.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    SetDllDirectory("\\DinamycLib\\DinamycLib");
    Leaver leaver;
    std::string name;
    std::cout << "Введите имя: ";
    std::cin >> name;
    std::cout << leaver.leave(name) << std::endl;

    return 0;
}