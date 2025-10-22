#include <fstream>
#include <memory>
#include <string>

// 1. Разделяем большой интерфейс на специализированные
class HTMLPrintable {
public:
    virtual ~HTMLPrintable() = default;
    virtual std::string printAsHTML() const = 0;
};

class TextPrintable {
public:
    virtual ~TextPrintable() = default;
    virtual std::string printAsText() const = 0;
};

class JSONPrintable {
public:
    virtual ~JSONPrintable() = default;
    virtual std::string printAsJSON() const = 0;
};

// 2. Базовый класс данных без знаний о формате
class Data {
public:
    explicit Data(std::string data) : data_(std::move(data)) {}
    virtual ~Data() = default;

    const std::string& getData() const { return data_; }

private:
    std::string data_;
};

// 3. Специализированные классы для каждого формата
// Каждый класс реализует ТОЛЬКО те интерфейсы, которые ему нужны

class TextData : public Data, public TextPrintable {
public:
    explicit TextData(std::string data) : Data(std::move(data)) {}

    std::string printAsText() const override {
        return getData();
    }
};

class HTMLData : public Data, public HTMLPrintable {
public:
    explicit HTMLData(std::string data) : Data(std::move(data)) {}

    std::string printAsHTML() const override {
        return "<html>" + getData() + "</html>";
    }
};

class JSONData : public Data, public JSONPrintable {
public:
    explicit JSONData(std::string data) : Data(std::move(data)) {}

    std::string printAsJSON() const override {
        return "{ \"data\": \"" + getData() + "\"}";
    }
};

// 4. Универсальные функции сохранения
// Они ничего не знают о конкретных форматах данных, только об интерфейсах

void saveToAsHTML(std::ofstream& file, const HTMLPrintable& printable) {
    file << printable.printAsHTML();
}

void saveToAsJSON(std::ofstream& file, const JSONPrintable& printable) {
    file << printable.printAsJSON();
}

void saveToAsText(std::ofstream& file, const TextPrintable& printable) {
    file << printable.printAsText();
}

// 5. Универсальная фабрика для создания данных
class DataFactory {
public:
    static std::unique_ptr<TextData> createTextData(const std::string& data) {
        return std::make_unique<TextData>(data);
    }

    static std::unique_ptr<HTMLData> createHTMLData(const std::string& data) {
        return std::make_unique<HTMLData>(data);
    }

    static std::unique_ptr<JSONData> createJSONData(const std::string& data) {
        return std::make_unique<JSONData>(data);
    }
};

// Пример использования
int main() {
    auto textData = DataFactory::createTextData("Hello, World!");
    auto htmlData = DataFactory::createHTMLData("Hello, World!");
    auto jsonData = DataFactory::createJSONData("Hello, World!");

    {
        std::ofstream file("text.txt");
        saveToAsText(file, *textData);  // Работает корректно
    }

    {
        std::ofstream file("data.html");
        saveToAsHTML(file, *htmlData);  // Работает корректно
    }

    {
        std::ofstream file("data.json");
        saveToAsJSON(file, *jsonData);  // Работает корректно
    }


    return 0;
}