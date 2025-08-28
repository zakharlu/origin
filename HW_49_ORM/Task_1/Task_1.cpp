#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/collection.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <Wt/Dbo/Session.h>
#include <Wt/Dbo/ptr.h>
#include <string>
#include <vector>
#include <iostream>     
#include <memory>
#include <algorithm>

using namespace Wt::Dbo;
using namespace std;

// Простые классы
class Publisher {
public:
    string name;

    Publisher() : name("") {}
    ~Publisher() {}

    template<class Action>
    void persist(Action& a) {
        field(a, name, "name");
    }
};

class Book {
public:
    string title;
    ptr<Publisher> publisher;

    Book() : title(""), publisher(nullptr) {}
    ~Book() {}

    template<class Action>
    void persist(Action& a) {
        field(a, title, "title");
        belongsTo(a, publisher, "id_publisher");
    }
};

class Shop {
public:
    string name;

    Shop() : name("") {}
    ~Shop() {}

    template<class Action>
    void persist(Action& a) {
        field(a, name, "name");
    }
};

class Stock {
public:
    ptr<Book> book;
    ptr<Shop> shop;
    int count;

    Stock() : book(nullptr), shop(nullptr), count(0) {}
    ~Stock() {}

    template<class Action>
    void persist(Action& a) {
        belongsTo(a, book, "id_book");
        belongsTo(a, shop, "id_shop");
        field(a, count, "count");
    }
};

class DatabaseManager {
private:
    unique_ptr<backend::Postgres> connection;
    Session session;

public:
    DatabaseManager(const string& connStr) {
        try {
            connection = make_unique<backend::Postgres>(connStr);
            session.setConnection(move(connection));

            session.mapClass<Publisher>("publisher");
            session.mapClass<Book>("book");
            session.mapClass<Shop>("shop");
            session.mapClass<Stock>("stock");

            Transaction transaction(session);
            session.createTables();
            transaction.commit();

            cout << "Успешно!" << endl;
        }
        catch (const exception& e) {
            cerr << "Ошибка: " << e.what() << endl;
            throw;
        }
    }

    void populateTestData() {
        Transaction transaction(session);

        try {
            auto publisher1 = session.addNew<Publisher>();
            publisher1.modify()->name = "Издатель 1";

            auto publisher2 = session.addNew<Publisher>();
            publisher2.modify()->name = "Издатель 2";

            auto book1 = session.addNew<Book>();
            book1.modify()->title = "Красная Шапочка";
            book1.modify()->publisher = publisher1;

            auto book2 = session.addNew<Book>();
            book2.modify()->title = "Война и имр";
            book2.modify()->publisher = publisher2;

            auto shop1 = session.addNew<Shop>();
            shop1.modify()->name = "Озон";

            auto shop2 = session.addNew<Shop>();
            shop2.modify()->name = "Букинист";

            auto stock1 = session.addNew<Stock>();
            stock1.modify()->book = book1;
            stock1.modify()->shop = shop1;
            stock1.modify()->count = 15;

            auto stock2 = session.addNew<Stock>();
            stock2.modify()->book = book2;
            stock2.modify()->shop = shop2;
            stock2.modify()->count = 8;

            transaction.commit();
            cout << "Тест успешный!" << endl;
        }
        catch (const exception& e) {
            cerr << "Ошибка: " << e.what() << endl;
            transaction.rollback();
            throw;
        }
    }

    void findShopsByPublisher(const string& publisherName) {
        try {
            Transaction transaction(session);

            cout << "\nМагазины с книгами '" << publisherName << "':" << endl;
            cout << "==================================" << endl;

           
            collection<ptr<Publisher>> publishers = session.find<Publisher>();
            collection<ptr<Book>> books = session.find<Book>();
            collection<ptr<Stock>> stocks = session.find<Stock>();

            // Шаг 1: Находим целевого издателя
            ptr<Publisher> targetPublisher;
            for (auto it = publishers.begin(); it != publishers.end(); ++it) {
                if ((*it)->name == publisherName) {
                    targetPublisher = *it;
                    break;
                }
            }

            if (!targetPublisher) {
                cout << "Издатель не найден: " << publisherName << endl;
                transaction.commit();
                return;
            }

            // Шаг 2: Находим книги этого издателя
            vector<long long> publisherBookIds;
            for (auto it = books.begin(); it != books.end(); ++it) {
                if ((*it)->publisher && (*it)->publisher.id() == targetPublisher.id()) {
                    publisherBookIds.push_back((*it).id());
                }
            }

            // Шаг 3: Находим магазины с этими книгами
            vector<string> foundShops;
            for (auto it = stocks.begin(); it != stocks.end(); ++it) {
                if ((*it)->book && (*it)->shop) {
                    for (long long bookId : publisherBookIds) {
                        if ((*it)->book.id() == bookId) {
                            foundShops.push_back((*it)->shop->name);
                            break;
                        }
                    }
                }
            }

            // Удаляем дубликаты
            sort(foundShops.begin(), foundShops.end());
            foundShops.erase(unique(foundShops.begin(), foundShops.end()), foundShops.end());

            if (foundShops.empty()) {
                cout << "No shops found for publisher: " << publisherName << endl;
            }
            else {
                for (const auto& shopName : foundShops) {
                    cout << "- " << shopName << endl;
                }
            }

            transaction.commit();
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
            throw;
        }
    }

    void listPublishers() {
        try {
            Transaction transaction(session);

            cout << "\nAvailable publishers:" << endl;
            cout << "=====================" << endl;

            // Объявление типа collection
            collection<ptr<Publisher>> publishers = session.find<Publisher>();

            // Объявление типа итератора
            collection<ptr<Publisher>>::iterator it = publishers.begin();
            collection<ptr<Publisher>>::iterator end = publishers.end();

            bool found = false;
            while (it != end) {
                cout << "- " << (*it)->name << endl;
                found = true;
                ++it;
            }

            if (!found) {
                cout << "Издатели не найдены." << endl;
            }

            transaction.commit();
        }
        catch (const exception& e) {
            cerr << "Ошибка: " << e.what() << endl;
            throw;
        }
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    try {
        string connStr = "host=localhost port=5432 dbname=bookstore user=postgres password=123";

        DatabaseManager db(connStr);
        db.populateTestData();
        db.listPublishers();

        string publisherName;
        cout << "\nВведите название издателя: ";
        getline(cin, publisherName);

        db.findShopsByPublisher(publisherName);

    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    cout << "Программа выполнена успешно!" << endl;
    return 0;
}