#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <stdexcept>
#include <locale>

// Типы сообщений
enum class Type {
    WARNING,
    ERROR,
    FATAL_ERROR,
    UNKNOWN
};

// Класс сообщения логирования
class LogMessage {
private:
    Type type_;
    std::string message_;

public:
    LogMessage(Type type, const std::string& message)
        : type_(type), message_(message) {
    }

    Type type() const {
        return type_;
    }

    const std::string& message() const {
        return message_;
    }
};

// Базовый класс обработчика
class LogHandler {
protected:
    std::unique_ptr<LogHandler> next_handler_;

public:
    virtual ~LogHandler() = default;

    void setNext(std::unique_ptr<LogHandler> next) {
        next_handler_ = std::move(next);
    }

    virtual void handle(const LogMessage& message) {
        if (next_handler_) {
            next_handler_->handle(message);
        }
    }
};

// Обработчик фатальных ошибок
class FatalErrorHandler : public LogHandler {
public:
    void handle(const LogMessage& message) override {
        if (message.type() == Type::FATAL_ERROR) {
            // Бросаем исключение с текстом сообщения
            throw std::runtime_error("Фатальная ошибка: " + message.message());
        }
        else {
            // Передаем следующему обработчику
            LogHandler::handle(message);
        }
    }
};

// Обработчик обычных ошибок
class ErrorHandler : public LogHandler {
private:
    std::string file_path_;

public:
    explicit ErrorHandler(const std::string& file_path)
        : file_path_(file_path) {
    }

    void handle(const LogMessage& message) override {
        if (message.type() == Type::ERROR) {
            // Записываем ошибку в файл
            std::ofstream file(file_path_, std::ios::app);
            if (file.is_open()) {
                file << "Ошибка: " << message.message() << std::endl;
                file.close();
            }
            else {
                throw std::runtime_error("Не удалось открыть файл для записи ошибок: " + file_path_);
            }
        }
        else {
            // Передаем следующему обработчику
            LogHandler::handle(message);
        }
    }
};

// Обработчик предупреждений
class WarningHandler : public LogHandler {
public:
    void handle(const LogMessage& message) override {
        if (message.type() == Type::WARNING) {
            // Печатаем предупреждение в консоль
            std::cout << "Предупреждение: " << message.message() << std::endl;
        }
        else {
            // Передаем следующему обработчику
            LogHandler::handle(message);
        }
    }
};

// Обработчик неизвестных сообщений
class UnknownMessageHandler : public LogHandler {
public:
    void handle(const LogMessage& message) override {
        if (message.type() == Type::UNKNOWN) {
            // Бросаем исключение о необработанном сообщении
            throw std::runtime_error("Необработанное сообщение неизвестного типа: " + message.message());
        }
        else {
            // Передаем следующему обработчику
            LogHandler::handle(message);
        }
    }
};

// Класс для построения цепочки обработчиков
class LogChainBuilder {
public:
    static std::unique_ptr<LogHandler> buildChain(const std::string& error_log_path) {
        // Создаем обработчики в правильном порядке приоритета
        auto unknown_handler = std::make_unique<UnknownMessageHandler>();
        auto warning_handler = std::make_unique<WarningHandler>();
        auto error_handler = std::make_unique<ErrorHandler>(error_log_path);
        auto fatal_handler = std::make_unique<FatalErrorHandler>();

        // Строим цепочку в порядке приоритета: 
        // Unknown -> Warning -> Error -> Fatal
        warning_handler->setNext(std::move(fatal_handler));
        error_handler->setNext(std::move(warning_handler));
        unknown_handler->setNext(std::move(error_handler));

        return unknown_handler;
    }
};

// Альтернативный упрощенный вариант построения цепочки
class SimpleLogChainBuilder {
public:
    static std::unique_ptr<LogHandler> buildChain(const std::string& error_log_path) {
        // Создаем цепочку с конца
        auto chain = std::make_unique<FatalErrorHandler>();

        auto error_handler = std::make_unique<ErrorHandler>(error_log_path);
        error_handler->setNext(std::move(chain));

        auto warning_handler = std::make_unique<WarningHandler>();
        warning_handler->setNext(std::move(error_handler));

        auto unknown_handler = std::make_unique<UnknownMessageHandler>();
        unknown_handler->setNext(std::move(warning_handler));

        return unknown_handler;
    }
};


// Демонстрация использования
int main() {
    setlocale(LC_ALL, "Russian");

    try {
        // Строим цепочку обработчиков
        auto log_chain = SimpleLogChainBuilder::buildChain("errors.log");

        std::cout << "=== Тестирование системы логирования ===\n" << std::endl;

        // Тестируем разные типы сообщений
        LogMessage warning_msg(Type::WARNING, "Низкий уровень заряда батареи");
        LogMessage error_msg(Type::ERROR, "Не удалось подключиться к базе данных");
        LogMessage fatal_msg(Type::FATAL_ERROR, "Критическая ошибка памяти");
        LogMessage unknown_msg(Type::UNKNOWN, "Непонятное сообщение");

        // Обрабатываем сообщения
        std::cout << "Обработка предупреждения:" << std::endl;
        log_chain->handle(warning_msg);

        std::cout << "\nОбработка ошибки:" << std::endl;
        log_chain->handle(error_msg);

        std::cout << "\nОбработка фатальной ошибки:" << std::endl;
        try {
            log_chain->handle(fatal_msg);
        }
        catch (const std::exception& e) {
            std::cout << "Поймано исключение: " << e.what() << std::endl;
        }

        std::cout << "\nОбработка неизвестного сообщения:" << std::endl;
        try {
            log_chain->handle(unknown_msg);
        }
        catch (const std::exception& e) {
            std::cout << "Поймано исключение: " << e.what() << std::endl;
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка в работе системы логирования: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}