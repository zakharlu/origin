#include <iostream>
#include "Counter.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    std::string choice = " ";
    int cnt_val = 0;

    std::cout << "Вы хотите указать начальное значение счётчика? Введите yes или no: ";
    std::cin >> choice;

    if (choice == "no") {
        cnt_val = 1;
    }
    else if (choice == "yes") {
        std::cout << "Введите начальное значение счетчика: ";
        std::cin >> cnt_val;
    }
    else {
        return 0;
    }


    Counter counter(cnt_val);

    char command = ' ';
    while (command != 'x') {
        std::cout << "Введите команду ('+', '-', '=' или 'x'): ";
        std::cin >> command;

        switch (command) {
        case '+':
            counter.increment();
            break;
        case '-':
            counter.decrement();
            break;
        case '=':
            counter.print();
            break;
        case 'x':
            std::cout << "До свидания!" << std::endl;
            break;
        default:
            std::cout << "Неверная команда" << std::endl;
            break;
        }
    }

    return 0;


}