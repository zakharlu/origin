#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

class Address {
    std::string city;
    std::string street;
    int house;
    int flat;

public:
    Address(std::string city, std::string street, int house, int flat) {
        this->city = city;
        this->street = street;
        this->house = house;
        this->flat = flat;
    }

    Address() {};

    std::string get_output_address() {
        return city + ", " + street + ", " + std::to_string(house) + ", " + std::to_string(flat);
    }

};

int main()
{
    setlocale(LC_ALL, "Russian");
    std::ifstream input_file("./TXT/in.txt");
    if (!input_file.is_open())
    {
        std::cout << "Ошибка открытия файла in.txt" << std::endl;
        system("pause");
        return 1;
    }

    std::ofstream output_file("./TXT/out.txt");
    if (!output_file.is_open())
    {
        std::cout << "Ошибка создания/открытия файла out.txt" << std::endl;
        system("pause");
        return 1;
    }

    std::string str_N, city, street, house, flat;
    int N = 0;
    std::getline(input_file, str_N);
    N = stoi(str_N);

    Address* arr_addr = new Address[N];
    for (int i = 0; i < N; ++i) {
        std::getline(input_file, city);
        std::getline(input_file, street);
        std::getline(input_file, house);
        std::getline(input_file, flat);
        arr_addr[i] = Address(city, street, stoi(house), stoi(flat));

    }

    output_file << N << std::endl;
    for (int i = N - 1; i >= 0; --i) {
        if (i != 0) {
            output_file << arr_addr[i].get_output_address() << std::endl;
        }
        else {
            output_file << arr_addr[i].get_output_address();
        }
    }

    delete[] arr_addr;
    input_file.close();
    output_file.close();

    return 0;
}