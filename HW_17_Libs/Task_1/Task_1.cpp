#include <iostream>
#include "maths.h"

void operation(int* num_1, int* num_2, int* menu);

int main() {
    int a, b, menu;
    setlocale(LC_ALL, "Russian");
    std::cout << "Введите первое число: ";
    std::cin >> a;
    std::cout << "Введите второе число: ";
    std::cin >> b;
    std::cout << "Выберите операцию (1 - сложение, 2 вычитание, 3 - умножение, 4 - деление, 5 - возведение в степень): ";
    std::cin >> menu;
    operation(&a, &b, &menu);

    return 0;
}

void operation(int* num_1, int* num_2, int* menu) {
    switch (*menu) {
    case 1:
        std::cout << *num_1 << " + " << *num_2 << " = " << add(*num_1, *num_2) << std::endl;
        break;
    case 2:
        std::cout << *num_1 << " - " << *num_2 << " = " << subtract(*num_1, *num_2) << std::endl;
        break;
    case 3:
        std::cout << *num_1 << " * " << *num_2 << " = " << mult(*num_1, *num_2) << std::endl;
        break;
    case 4:
        std::cout << *num_1 << " / " << *num_2 << " = " << division(*num_1, *num_2) << std::endl;
        break;
    case 5:
        std::cout << *num_1 << " в степени " << *num_2 << " = " << sqrt(*num_1, *num_2) << std::endl;
        break;
    }
}