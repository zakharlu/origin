#include "transport.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

void printMainMenu() {
    std::cout << "\nГоночный симулятор" << std::endl;
    std::cout << "1. Гонка для наземного транспорта" << std::endl;
    std::cout << "2. Гонка для воздушного транспорта" << std::endl;
    std::cout << "3. Гонка для наземного и воздушного транспорта" << std::endl;
    std::cout << "Выберите тип гонки: ";
}

void printTransportList(const std::vector<Transport*>& transports,
    const std::vector<bool>& registered) {
    for (size_t i = 0; i < transports.size(); ++i) {
        std::cout << i + 1 << ". " << transports[i]->getName();
        if (registered[i]) std::cout << " (зарегистрирован)";
        std::cout << std::endl;
    }
}

bool isValidForRace(Transport* t, TransportType raceType) {
    return raceType == TransportType::GROUND && t->getType() == TransportType::GROUND ||
        raceType == TransportType::AIR && t->getType() == TransportType::AIR ||
        raceType == TransportType::MIXED;
}

void conductRace(const std::vector<Transport*>& raceTransports, int distance) {
    std::vector<std::pair<std::string, double>> results;

    // Рассчитываем результаты
    for (auto t : raceTransports) {
        results.emplace_back(t->getName(), t->calcTimeRide(distance));
    }

    // Сортируем по времени
    std::sort(results.begin(), results.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });

    // Выводим результаты
    std::cout << "\n=== РЕЗУЛЬТАТЫ ГОНКИ ===" << std::endl;
    std::cout << "Дистанция: " << distance << " км" << std::endl;
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << i + 1 << ". " << results[i].first
            << ": " << results[i].second << " ч" << std::endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    // Инициализация транспорта
    size_t totalTransports = 0;
    Transport** allTransports = makeTransports(totalTransports);
    std::vector<Transport*> transportsList(allTransports, allTransports + totalTransports);

    while (true) {
        // Выбор типа гонки
        TransportType raceType;
        while (true) {
            printMainMenu();
            int typeChoice;
            std::cin >> typeChoice;

            if (typeChoice == 1) { raceType = TransportType::GROUND; break; }
            if (typeChoice == 2) { raceType = TransportType::AIR; break; }
            if (typeChoice == 3) { raceType = TransportType::MIXED; break; }
        }

        // Ввод дистанции
        int distance;
        std::cout << "\n Укажите длину дистанции (должна быть положительна): ";
        std::cin >> distance;
        while (distance <= 0) {
            std::cout << "Дистанция должна быть положительной. Повторите ввод: ";
            std::cin >> distance;
        }

        // Регистрация транспорта
        std::vector<Transport*> raceTransports;
        std::vector<bool> registered(totalTransports, false);

        while (true) {
            system("cls");
            std::cout << std::endl;
            std::cout << (raceType == TransportType::GROUND ? "Гонка для наземного транспорта" :
                    raceType == TransportType::AIR ? "Гонка для воздушного транспорта" : "Гонка для смешанного транспорта")
                << std::endl;
            std::cout << "Расстояние: " << distance << " км" << std::endl;
            std::cout << "\nДоступные транспортные средства:" << std::endl;

            printTransportList(transportsList, registered);

            std::cout << "0. Закончить регистрацию. Начать гонку" << std::endl;
            std::cout << "Выберите транспорт или 0 для окончания процесса регистрации и начала гонки: ";

            int transportChoice;
            std::cin >> transportChoice;

            if (transportChoice == 0) {
                if (raceTransports.size() < 2) {
                    std::cout << "Необходимо зарегистрировать минимум 2 транспортных средства!" << std::endl;
                    system("pause");
                    continue;
                }
                break;
            }

            if (transportChoice < 0 || transportChoice > totalTransports) {
                continue;
            }

            size_t index = transportChoice - 1;
            Transport* selected = transportsList[index];

            if (registered[index]) {
                std::cout << "Это транспортное средство уже зарегистрировано!" << std::endl;
                system("pause");
                continue;
            }

            if (!isValidForRace(selected, raceType)) {
                std::cout << "Этот транспорт не может участвовать в данной гонке!" << std::endl;
                system("pause");
                continue;
            }

            registered[index] = true;
            raceTransports.push_back(selected);
            std::cout << selected->getName() << " успешно зарегистрирован!" << std::endl;
            system("pause");
        }

        int choice;

        // Проведение гонки
        conductRace(raceTransports, distance);
        system("pause");

        // Предложение продолжить
        std::cout << "\n1. Провести ещё одну гонку" << std::endl;
        std::cout << "2. Выйти" << std::endl;
        std::cout << "Выберите действие: ";
        std::cin >> choice;
        if (choice == 2) break;
    }

    // Очистка памяти
    for (size_t i = 0; i < totalTransports; ++i) {
        delete allTransports[i];
    }
    delete[] allTransports;

    return 0;
}