#include <iostream>

struct address
{
    std::string city;
    std::string street;
    std::string house;
    int flat;
    int index;
};

void print_addr(address* addr);

int main()
{
    setlocale(LC_ALL, "Russian");
    address addr_1{ "Москва", "Арбат", "12", 8, 123456 };
    print_addr(&addr_1);
    address addr_2{ "Ижевск", "Пушкина", "59", 143, 953769 };
    print_addr(&addr_2);
}

void print_addr(address* addr) {
    std::cout << "Город: " << addr->city << std::endl;
    std::cout << "Улица: " << addr->street << std::endl;
    std::cout << "Дом: " << addr->house << std::endl;
    std::cout << "Квартира: " << addr->flat << std::endl;
    std::cout << "Индекс: " << addr->index << std::endl;
    std::cout << std::endl;
}