#include <iostream>

struct account
{
    int acc_num;
    std::string name;
    double acc_sum;
};

void update_sum(account* acc, double* sum);

int main()
{
    setlocale(LC_ALL, "Russian");
    account acc;
    double new_acc_sum = 0;
    std::cout << "Введите номер счета: ";
    std::cin >> acc.acc_num;
    std::cout << "Введите имя владельца: ";
    std::cin >> acc.name;
    std::cout << "Введите баланс: ";
    std::cin >> acc.acc_sum;
    std::cout << "Введите новый баланс: ";
    std::cin >> new_acc_sum;
    update_sum(&acc, &new_acc_sum);
    std::cout << "Ваш счет: " << acc.acc_num << ", " << acc.name << ", " << acc.acc_sum;
}

void update_sum(account* acc, double* sum) {
    acc->acc_sum = *sum;
}