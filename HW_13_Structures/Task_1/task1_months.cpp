#include <iostream>

enum class months
{
    january = 1,
    february,
    march,
    april,
    may,
    june,
    july,
    august,
    septenber,
    october,
    november,
    december
};

int main()
{   
    setlocale(LC_ALL, "Russian");
    int num= -1;
    while (num != 0) {
        std::cout << "Введите номер месяца: ";
        std::cin >> num;
        months month = static_cast<months>(num);
        switch (month) {
            case static_cast<months>(0):
                std::cout << "До свидания!" << std::endl;
                break;
            case static_cast<months>(1):
                std::cout << "Январь" << std::endl;
                break;
            case static_cast<months>(2):
                std::cout << "Февраль" << std::endl;
                break;
            case static_cast<months>(3):
                std::cout << "Март" << std::endl;
                break;
            case static_cast<months>(4):
                std::cout << "Апрель" << std::endl;
                break;
            case static_cast<months>(5):
                std::cout << "Май" << std::endl;
                break;
            case static_cast<months>(6):
                std::cout << "Июнь" << std::endl;
                break;
            case static_cast<months>(7):
                std::cout << "Июль" << std::endl;
                break;
            case static_cast<months>(8):
                std::cout << "Август" << std::endl;
                break;
            case static_cast<months>(9):
                std::cout << "Сентябрь" << std::endl;
                break;
            case static_cast<months>(10):
                std::cout << "Октябрь" << std::endl;
                break;
            case static_cast<months>(11):
                std::cout << "Ноябрь" << std::endl;
                break;
            case static_cast<months>(12):
                std::cout << "Декабрь" << std::endl;
                break;
            default:
                std::cout << "Неправильный номер!" << std::endl;
                break;
        }
    }
    return 0;
}