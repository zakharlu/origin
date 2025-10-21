#include <iostream>
#include <fstream>
#include <string>

class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

// Наследник для печати в консоль
class ConsoleLogCommand : public LogCommand {
public:
    void print(const std::string& message) override {
        std::cout << message << std::endl;
    }
};

// Наследник для печати в файл
class FileLogCommand : public LogCommand {
private:
    std::string file_path_;

public:
    explicit FileLogCommand(const std::string& file_path)
        : file_path_(file_path) {
    }

    void print(const std::string& message) override {
        std::ofstream file(file_path_, std::ios::app); // открываем файл в режиме добавления
        if (file.is_open()) {
            file << message << std::endl;
            file.close();
        }
        else {
            std::cerr << "Ошибка: не удалось открыть файл " << file_path_ << std::endl;
        }
    }
};

// Функция для выполнения команд
void print(LogCommand& logger, const std::string& message) {
    logger.print(message);
}


int main() {
    setlocale(LC_ALL, "Russian");
    // Печать в консоль
    ConsoleLogCommand console_logger;
    print(console_logger, "Это сообщение в консоли");

    // Печать в файл
    FileLogCommand file_logger("log.txt");
    print(file_logger, "Это сообщение в файле");

    return 0;
}