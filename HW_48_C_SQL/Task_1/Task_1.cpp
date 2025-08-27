#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <pqxx/pqxx>
#include <stdexcept>

class ClientManager {
private:
    std::unique_ptr<pqxx::connection> conn;

public:
    ClientManager(const std::string& connection_string) {
        try {
            conn = std::make_unique<pqxx::connection>(connection_string);
            if (!conn->is_open()) {
                throw std::runtime_error("Cannot open database connection");
            }
        }
        catch (const pqxx::sql_error& e) {
            throw std::runtime_error("Database connection error: " + std::string(e.what()));
        }
    }

    // Метод, создающий структуру БД
    void createTables() {
        try {
            pqxx::work txn(*conn);

            // Таблица клиентов
            txn.exec(
                "CREATE TABLE IF NOT EXISTS clients ("
                "id SERIAL PRIMARY KEY,"
                "first_name VARCHAR(100) NOT NULL,"
                "last_name VARCHAR(100) NOT NULL,"
                "email VARCHAR(255) UNIQUE NOT NULL"
                ")"
            );

            // Таблица телефонов
            txn.exec(
                "CREATE TABLE IF NOT EXISTS phones ("
                "id SERIAL PRIMARY KEY,"
                "client_id INTEGER NOT NULL REFERENCES clients(id) ON DELETE CASCADE,"
                "phone_number VARCHAR(20) NOT NULL"
                ")"
            );

            txn.commit();
            std::cout << "Таблицы созданы успешно." << std::endl;

        }
        catch (const pqxx::sql_error& e) {
            throw std::runtime_error("SQL error in createTables: " + std::string(e.what()));
        }
    }

    // Метод, позволяющий добавить нового клиента
    int addClient(const std::string& firstName, const std::string& lastName, const std::string& email) {
        try {
            pqxx::work txn(*conn);

            pqxx::result result = txn.exec_params(
                "INSERT INTO clients (first_name, last_name, email) VALUES ($1, $2, $3) RETURNING id",
                firstName, lastName, email
            );

            int clientId = result[0][0].as<int>();
            txn.commit();

            std::cout << "Добавлен клиент с ID: " << clientId << std::endl;
            return clientId;

        }
        catch (const pqxx::sql_error& e) {
            throw std::runtime_error("SQL error in addClient: " + std::string(e.what()));
        }
    }

    // Метод, позволяющий добавить телефон для существующего клиента
    void addPhone(int clientId, const std::string& phoneNumber) {
        try {
            pqxx::work txn(*conn);

            txn.exec_params(
                "INSERT INTO phones (client_id, phone_number) VALUES ($1, $2)",
                clientId, phoneNumber
            );

            txn.commit();
            std::cout << "Добавлен телефон для клиента ID: " << clientId << std::endl;

        }
        catch (const pqxx::sql_error& e) {
            throw std::runtime_error("Ошибка SQL в addPhone: " + std::string(e.what()));
        }
    }

    // Метод, позволяющий изменить данные о клиенте
    void updateClient(int clientId, const std::string& firstName, const std::string& lastName, const std::string& email) {
        try {
            pqxx::work txn(*conn);

            txn.exec_params(
                "UPDATE clients SET first_name = $1, last_name = $2, email = $3 WHERE id = $4",
                firstName, lastName, email, clientId
            );

            txn.commit();
            std::cout << "Обновлен клиент: " << clientId << std::endl;

        }
        catch (const pqxx::sql_error& e) {
            throw std::runtime_error("Ошибка SQL в updateClient: " + std::string(e.what()));
        }
    }

    // Метод, позволяющий удалить телефон у существующего клиента
    void deletePhone(int phoneId) {
        try {
            pqxx::work txn(*conn);

            txn.exec_params(
                "DELETE FROM phones WHERE id = $1",
                phoneId
            );

            txn.commit();
            std::cout << "Удален телефон: " << phoneId << std::endl;

        }
        catch (const pqxx::sql_error& e) {
            throw std::runtime_error("Ошибка SQL в deletePhone: " + std::string(e.what()));
        }
    }

    // Метод, позволяющий удалить существующего клиента
    void deleteClient(int clientId) {
        try {
            pqxx::work txn(*conn);

            txn.exec_params(
                "DELETE FROM clients WHERE id = $1",
                clientId
            );

            txn.commit();
            std::cout << "Удален клиент: " << clientId << std::endl;

        }
        catch (const pqxx::sql_error& e) {
            throw std::runtime_error("Ошибка SQL в deleteClient: " + std::string(e.what()));
        }
    }

    // Структура для хранения информации о клиенте
    struct ClientInfo {
        int id;
        std::string firstName;
        std::string lastName;
        std::string email;
        std::vector<std::pair<int, std::string>> phones; // (phone_id, phone_number)
    };

    // Метод, позволяющий найти клиента по его данным
    std::vector<ClientInfo> findClient(const std::string& searchTerm) {
        try {
            pqxx::work txn(*conn);

            std::string searchPattern = "%" + searchTerm + "%";

            pqxx::result result = txn.exec_params(
                "SELECT c.id, c.first_name, c.last_name, c.email, p.id as phone_id, p.phone_number "
                "FROM clients c "
                "LEFT JOIN phones p ON c.id = p.client_id "
                "WHERE c.first_name ILIKE $1 OR c.last_name ILIKE $2 OR c.email ILIKE $3 OR p.phone_number ILIKE $4 "
                "ORDER BY c.id",
                searchPattern, searchPattern, searchPattern, searchPattern
            );

            std::vector<ClientInfo> results;
            int currentClientId = -1;
            ClientInfo currentClient;

            for (const auto& row : result) {
                int clientId = row["id"].as<int>();

                if (clientId != currentClientId) {
                    if (currentClientId != -1) {
                        results.push_back(currentClient);
                    }

                    currentClientId = clientId;
                    currentClient = ClientInfo{
                        clientId,
                        row["first_name"].as<std::string>(),
                        row["last_name"].as<std::string>(),
                        row["email"].as<std::string>(),
                        {}
                    };
                }

                // Добавляем телефон, если он есть
                if (!row["phone_id"].is_null()) {
                    int phoneId = row["phone_id"].as<int>();
                    std::string phoneNumber = row["phone_number"].as<std::string>();
                    currentClient.phones.push_back({ phoneId, phoneNumber });
                }
            }

            // Добавляем последнего клиента
            if (currentClientId != -1) {
                results.push_back(currentClient);
            }

            return results;

        }
        catch (const pqxx::sql_error& e) {
            throw std::runtime_error("Ошибка SQL в findClient: " + std::string(e.what()));
        }
    }

    // Вспомогательный метод для вывода информации о клиенте
    static void printClientInfo(const ClientInfo& client) {
        std::cout << "ID: " << client.id << std::endl;
        std::cout << "Имя, фамилия: " << client.firstName << " " << client.lastName << std::endl;
        std::cout << "Email: " << client.email << std::endl;
        std::cout << "Телефон: ";
        if (client.phones.empty()) {
            std::cout << "Нет" << std::endl;
        }
        else {
            for (const auto& phone : client.phones) {
                std::cout << phone.second << " (ID: " << phone.first << "), ";
            }
            std::cout << std::endl;
        }
        std::cout << "-------------------" << std::endl;
    }

    // Дополнительный метод для получения всех клиентов
    std::vector<ClientInfo> getAllClients() {
        try {
            pqxx::work txn(*conn);

            pqxx::result result = txn.exec(
                "SELECT c.id, c.first_name, c.last_name, c.email, p.id as phone_id, p.phone_number "
                "FROM clients c "
                "LEFT JOIN phones p ON c.id = p.client_id "
                "ORDER BY c.id"
            );


            std::vector<ClientInfo> results;
            int currentClientId = -1;
            ClientInfo currentClient;

            for (const auto& row : result) {
                int clientId = row["id"].as<int>();

                if (clientId != currentClientId) {
                    if (currentClientId != -1) {
                        results.push_back(currentClient);
                    }

                    currentClientId = clientId;
                    currentClient = ClientInfo{
                        clientId,
                        row["first_name"].as<std::string>(),
                        row["last_name"].as<std::string>(),
                        row["email"].as<std::string>(),
                        {}
                    };
                }

                // Добавляем телефон, если он есть
                if (!row["phone_id"].is_null()) {
                    int phoneId = row["phone_id"].as<int>();
                    std::string phoneNumber = row["phone_number"].as<std::string>();
                    currentClient.phones.push_back({ phoneId, phoneNumber });
                }
            }

            // Добавляем последнего клиента
            if (currentClientId != -1) {
                results.push_back(currentClient);
            }

            return results;

        }
        catch (const pqxx::sql_error& e) {
            throw std::runtime_error("Ошибка SQL в getAllClients: " + std::string(e.what()));
        }
    }
};


int main() {
    setlocale(LC_ALL, "Russian");
    try {
        // Строка подключения к PostgreSQL
        std::string connection_string =
            "host=localhost "
            "port=5432 "
            "dbname=db_client "
            "user=postgres "
            "password=123";

        ClientManager manager(connection_string);

        // Создаем таблицы
        manager.createTables();

        // Добавляем клиентов
        int client1 = manager.addClient("Ivan", "Ivanov", "ivan.ivanov@email.com");
        int client2 = manager.addClient("Petr", "Petrov", "petr.petrov@email.com");
        int client3 = manager.addClient("Sidor", "Sidorov", "sidor.sidorov@email.com");

        // Добавляем телефоны
        manager.addPhone(client1, "+1234567890");
        manager.addPhone(client1, "+0987654321");
        manager.addPhone(client2, "+1112223333");
        manager.addPhone(client3, "+4445556666");
        manager.addPhone(client3, "+7778889999");

        // Показываем всех клиентов
        std::cout << "\nВсе клиенты:" << std::endl;
        auto allClients = manager.getAllClients();
        for (const auto& client : allClients) {
            ClientManager::printClientInfo(client);
        }

        // Ищем клиентов
        std::cout << "Поиск 'Ivan':" << std::endl;
        auto results = manager.findClient("Ivan");
        for (const auto& client : results) {
            ClientManager::printClientInfo(client);
        }

        std::cout << "Поиск '111':" << std::endl;
        results = manager.findClient("111");
        for (const auto& client : results) {
            ClientManager::printClientInfo(client);
        }

        // Обновляем данные клиента
        manager.updateClient(client1, "Ivan", "Ivanov", "ivan.new@email.ru");

        // Снова ищем
        std::cout << "Поиск 'new':" << std::endl;
        results = manager.findClient("new");
        for (const auto& client : results) {
            ClientManager::printClientInfo(client);
        }

        // Удаляем клиента (телефоны тоже удалятся каскадно)
        manager.deleteClient(client2);

        // Проверяем, что клиент удален
        std::cout << "Поиск 'Petrov':" << std::endl;
        results = manager.findClient("Petrov");
        if (results.empty()) {
            std::cout << "Нет клиентов 'Petrov'." << std::endl;
        }

        // Показываем оставшихся клиентов
        std::cout << "\nОставшиеся клиенты:" << std::endl;
        allClients = manager.getAllClients();
        for (const auto& client : allClients) {
            ClientManager::printClientInfo(client);
        }

    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}