#include <iostream>

class Counter {
    int cnt_val;

public:
    Counter(int cnt_val) {
        this->cnt_val = cnt_val;
    }
    
    void update_cnt(int increment) {
            cnt_val += increment;
        }

    void print() {
        std::cout << cnt_val << std::endl;
    }
};

int main()
{
    std::string choice = " ";
    int cnt_val = 0;
    setlocale(LC_ALL, "Russian");
    
    std::cout << "Вы хотите указать начальное значение счётчика? Введите да или нет: ";
    std::cin >> choice;
    
    if (choice == "no") {
        cnt_val = 1;
    } else if (choice == "yes") {
        std::cout << "Введите начальное значение счетчика: ";
        std::cin >> cnt_val;
    } else {
        return 0;
    }

        
    Counter counter(cnt_val);

    char command = ' ';
    while (command != 'x') {
        std::cout << "Введите команду: ";
        std::cin >> command;

        switch (command) {
        case '+':
            counter.update_cnt(1);
            break;
        case '-':
            counter.update_cnt(-1);
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
