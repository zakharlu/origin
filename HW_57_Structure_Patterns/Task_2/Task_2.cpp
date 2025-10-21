#include <map>
#include <string>
#include <iostream>
#include <memory>

using namespace std;

class VeryHeavyDatabase {
public:
    virtual std::string get_data(const std::string& key) {
        return "value";
    }
};

class OneShotDB : public VeryHeavyDatabase {
private:
    VeryHeavyDatabase* real_object;
    size_t shots;
    size_t counter;

public:
    explicit OneShotDB(VeryHeavyDatabase* real_object, size_t shots = 1)
        : real_object(real_object), shots(shots), counter(0) {
    }

    std::string get_data(const std::string& key) override {
        if (counter < shots) {
            counter++;
            return real_object->get_data(key);
        }
        else {
            return "error";
        }
    }
};

class CacheDatabase : public VeryHeavyDatabase {
    map<string, string> __cache;
    unique_ptr<VeryHeavyDatabase> __real_db;

public:
    CacheDatabase(unique_ptr<VeryHeavyDatabase> real_db) : __real_db(move(real_db)) {}

    std::string get_data(const std::string& key) override {
        if (__cache.find(key) == __cache.end()) {
            cout << "Get from real DB" << endl;
            __cache[key] = __real_db->get_data(key);
        }
        else {
            cout << "Get from cache" << endl;
        }
        return __cache[key];
    }
};

class MockDatabase : public VeryHeavyDatabase {
    unique_ptr<VeryHeavyDatabase> __real_db;

public:
    MockDatabase(unique_ptr<VeryHeavyDatabase> real_db) : __real_db(move(real_db)) {}

    std::string get_data(const std::string& key) override {
        return "Correct data";  // Всегда возвращает "Correct data"
    }
};

unique_ptr<CacheDatabase> init_cache() {
    auto original_DB = make_unique<VeryHeavyDatabase>();
    return make_unique<CacheDatabase>(move(original_DB));
}

int main()
{
    // Тестирование OneShotDB
    auto real_db = VeryHeavyDatabase();
    auto limit_db = OneShotDB(std::addressof(real_db), 2);
    std::cout << limit_db.get_data("key") << std::endl;
    std::cout << limit_db.get_data("key") << std::endl;
    std::cout << limit_db.get_data("key") << std::endl;

    cout << endl;

    // Используем MockDatabase для получения "Correct data"
    auto test_db = make_unique<CacheDatabase>(move(make_unique<MockDatabase>(make_unique<VeryHeavyDatabase>())));
    cout << test_db->get_data("key2") << endl;
    cout << test_db->get_data("key3") << endl;
    cout << test_db->get_data("key4") << endl;
    cout << test_db->get_data("key5") << endl;
    cout << test_db->get_data("key6") << endl;
    cout << test_db->get_data("key7") << endl;

    return 0;
}