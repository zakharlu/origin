#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <locale>

// Базовый класс наблюдателя
class Observer {
public:
    virtual ~Observer() = default;
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

// Наблюдаемый класс
class Observed {
private:
    std::vector<Observer*> observers_;

    // Вспомогательный метод для безопасного оповещения наблюдателей
    template<typename Func, typename... Args>
    void notifyObservers(Func func, Args&&... args) const {
        // Создаем копию вектора для безопасной итерации
        auto observers_copy = observers_;

        for (auto observer : observers_copy) {
            if (observer && std::find(observers_.begin(), observers_.end(), observer) != observers_.end()) {
                // Вызываем метод наблюдателя, если он все еще в списке
                (observer->*func)(std::forward<Args>(args)...);
            }
        }
    }

public:
    virtual ~Observed() = default;

    // Добавление наблюдателя (не владеющий указатель)
    void addObserver(Observer* observer) {
        if (observer && std::find(observers_.begin(), observers_.end(), observer) == observers_.end()) {
            observers_.push_back(observer);
        }
    }

    // Удаление наблюдателя
    void removeObserver(Observer* observer) {
        auto it = std::find(observers_.begin(), observers_.end(), observer);
        if (it != observers_.end()) {
            observers_.erase(it);
        }
    }

    // Методы для генерации событий
    void warning(const std::string& message) const {
        std::cout << "[ПРЕДУПРЕЖДЕНИЕ] ";
        notifyObservers(&Observer::onWarning, message);
    }

    void error(const std::string& message) const {
        std::cout << "[ОШИБКА] ";
        notifyObservers(&Observer::onError, message);
    }

    void fatalError(const std::string& message) const {
        std::cout << "[КРИТИЧЕСКАЯ ОШИБКА] ";
        notifyObservers(&Observer::onFatalError, message);
    }
};

// Класс для работы с предупреждениями - печатает в консоль
class WarningObserver : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "Предупреждение: " << message << std::endl;
    }
};

// Класс для работы с ошибками - печатает в файл
class ErrorObserver : public Observer {
private:
    std::string file_path_;

public:
    explicit ErrorObserver(const std::string& file_path) : file_path_(file_path) {}

    void onError(const std::string& message) override {
        std::ofstream file(file_path_, std::ios::app);
        if (file.is_open()) {
            file << "Ошибка: " << message << std::endl;
            file.close();
        }
        else {
            std::cerr << "Не удалось открыть файл для записи ошибок: " << file_path_ << std::endl;
        }
    }
};

// Класс для работы с фатальными ошибками - печатает и в консоль, и в файл
class FatalErrorObserver : public Observer {
private:
    std::string file_path_;

public:
    explicit FatalErrorObserver(const std::string& file_path) : file_path_(file_path) {}

    void onFatalError(const std::string& message) override {
        // Печать в консоль
        std::cout << "Критическая ошибка: " << message << std::endl;

        // Печать в файл
        std::ofstream file(file_path_, std::ios::app);
        if (file.is_open()) {
            file << "Критическая ошибка: " << message << std::endl;
            file.close();
        }
        else {
            std::cerr << "Не удалось открыть файл для записи критических ошибок: " << file_path_ << std::endl;
        }
    }
};

// Функция для настройки локали (для корректного отображения русского языка в Windows)
void setupRussianLocale() {
#ifdef _WIN32
    setlocale(LC_ALL, "Russian");
#else
    std::locale::global(std::locale("ru_RU.UTF-8"));
#endif
}

// Демонстрация использования
int main() {

    setlocale(LC_ALL, "Russian");
    // Создаем наблюдаемый объект
    Observed observed;

    // Создаем наблюдателей
    WarningObserver warning_observer;
    ErrorObserver error_observer("ошибки.log");
    FatalErrorObserver fatal_observer("критические_ошибки.log");

    // Добавляем наблюдателей
    observed.addObserver(&warning_observer);
    observed.addObserver(&error_observer);
    observed.addObserver(&fatal_observer);

    // Генерируем события с русским текстом
    std::cout << "=== Тестирование системы логирования ===\n" << std::endl;

    observed.warning("Обнаружена некритичная проблема в работе системы");
    observed.error("Не удалось подключиться к базе данных");
    observed.fatalError("Произошло критическое исключение - работа приложения будет завершена");

    // Демонстрация безопасного удаления наблюдателя
    observed.removeObserver(&error_observer);
    std::cout << "\n=== После удаления ErrorObserver ===\n" << std::endl;
    observed.error("Эта ошибка не будет записана в файл");

    // Дополнительные тестовые сообщения
    std::cout << "\n=== Дополнительные тесты ===\n" << std::endl;
    observed.warning("Низкий уровень заряда батареи");
    observed.fatalError("Отсутствует подключение к интернету");

    return 0;
}