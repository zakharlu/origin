#include <iostream>

class Calculator {
    double num1, num2;
public:
    bool set_num1(double num1) {
        bool flag_value = false;
        if (num1 != 0) {
            this->num1 = num1;
            flag_value = true;
        }
        return flag_value;
    }

    bool set_num2(double num2) {
        bool flag_value = false;
        if (num2 != 0) {
            this->num2 = num2;
            flag_value = true;
        }
        return flag_value;
    }

    double add() {
        return num1 + num2;
    }
    
    double multiply() {
        return num1 * num2;
    }

    double msubtract_1_2() {
        return num1 - num2;
    }

    double msubtract_2_1() {
        return num2 - num1;
    }

    double divide_1_2() {
        return num1 / num2;
    }

    double divide_2_1() {
        return num2 / num1;
    }
};

int main()
{
    double num1 = 0, num2 = 0;
    Calculator calc;
    setlocale(LC_ALL, "Russian");
    std::cout << "Введите num1: ";
    std::cin >> num1;
    while (!calc.set_num1(num1)) {
        std::cout << "Неверный ввод!" << std::endl;
        std::cout << "Введите num1: ";
        std::cin >> num1;
    }

    std::cout << "Введите num2: ";
    std::cin >> num2;
    while (!calc.set_num2(num2)) {
        std::cout << "Неверный ввод!" << std::endl;
        std::cout << "Введите num2: ";
        std::cin >> num2;
    }

    std::cout << "num1 + num2 = " << calc.add() << std::endl;
    std::cout << "num1 * num2 = " << calc.multiply() << std::endl;
    std::cout << "num1 - num2 = " << calc.msubtract_1_2() << std::endl;
    std::cout << "num2 - num1 = " << calc.msubtract_2_1() << std::endl;
    std::cout << "num1 / num2 = " << calc.divide_1_2() << std::endl;
    std::cout << "num2 / num1 = " << calc.divide_2_1() << std::endl;
}