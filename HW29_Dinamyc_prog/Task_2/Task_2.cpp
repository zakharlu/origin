#include <iostream>

int fibonacci(int n);

int main()
{
    setlocale(LC_ALL, "Russian");
    int n = 0;
    std::cout << "Введите номер числа Фибоначчи от 0 до 9: ";
    std::cin >> n;
    int fib = 0;
    fib = fibonacci(n);
    std::cout << "Значение Фибоначчи: " << fib << std::endl;

}

int fibonacci(int n) {
    int arr[10] = { 0 };
    if (n <= 1) return n;
    else {
        if (arr[n] != 0) {
            return arr[n];
        }
        arr[n] = fibonacci(n - 1) + fibonacci(n - 2);
    }
}