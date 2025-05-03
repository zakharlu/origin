#include <iostream>

int fibonacci(int n);

int main()
{
    setlocale(LC_ALL, "Russian");
    int n = 0;
    std::cout << "Введите номер числа Фибоначчи: ";
    std::cin >> n;
    int fib = 0;
    fib = fibonacci(n);
    std::cout << "Значение Фибоначчи: " << fib << std::endl;

}

int fibonacci(int n) {
    if (n <= 1) return n;
    else return fibonacci(n - 1) + fibonacci(n - 2);
}