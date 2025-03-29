#define MODE 1
#ifndef MODE
    #error Требуется определить MODE
#elif MODE == 1
    #define add(type, num1, num2) (num1 + num2);
#endif
    

#include <iostream>


int main() {
    setlocale(LC_ALL, "Russian");
    #if defined MODE
        #if MODE == 0
            std::cout << "Работаю в боевом режиме" << std::endl;
        #elif MODE == 1
            int num1, num2;
            std::cout << "Работаю в боевом режиме" << std::endl;
            std::cout << "Введите число 1: ";
            std::cin >> num1;
            std::cout << "Введите число 2: ";
            std::cin >> num2;
            std::cout << "Результат сложения: " << add(int, num1, num2);
        #else
            std::cout << "Неизвестный режим. Завершение работы" << std::endl;
        #endif
    #endif
    return 0;
}