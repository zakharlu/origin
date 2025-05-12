#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <algorithm>
//#include "VehicleRacingLib.h"
#include "../VehicleRacingLib/Vehicle.h"
#include "../VehicleRacingLib/GroundVehicle.h"
#include "../VehicleRacingLib/AirVehicle.h"
#include "../VehicleRacingLib/Camel.h"
#include "../VehicleRacingLib/FastCamel.h"
#include "../VehicleRacingLib/Centaur.h"
#include "../VehicleRacingLib/AllTerrainBoots.h"
#include "../VehicleRacingLib/MagicCarpet.h"
#include "../VehicleRacingLib/Eagle.h"
#include "../VehicleRacingLib/Broom.h"
#include "../VehicleRacingLib/Race.h"

using namespace std;

// void printMainMenu();
void printRaceTypeMenu();
void printVehicleMenu(RaceType raceType);
shared_ptr<Vehicle> createVehicle(int choice);
RaceType getRaceTypeFromChoice(int choice);
string getRaceTypeName(RaceType type);

int main() {
    setlocale(LC_ALL, "Russian");

    while (true) {
        // Выбор типа гонки
        printRaceTypeMenu();
        int raceTypeChoice;
        cin >> raceTypeChoice;

        if (raceTypeChoice == 0) {
            break;
        }

        RaceType raceType = getRaceTypeFromChoice(raceTypeChoice);

        // Ввод дистанции
        double distance;
        cout << "Укажите длину дистанции (должна быть положительна): ";
        cin >> distance;
        while (distance <= 0) {
            cout << "Дистанция должна быть положительной. Попробуйте снова: ";
            cin >> distance;
        }

        // Создание гонки
        Race race(raceType, distance);

        // Регистрация транспортных средств
        while (true) {
            system("cls");
            cout << "Гонка для " << getRaceTypeName(raceType) << ". Дистанция: " << distance << endl;

            if (race.getVehicles().size() >= 2) {
                cout << "Зарегистрированные транспортные средства:" << endl;
                for (const auto& vehicle : race.getVehicles()) {
                    cout << " - " << vehicle->getName() << endl;
                }
                cout << "\n1. Зарегистрировать транспорт" << endl;
                cout << "2. Начать гонку" << endl;
                cout << "Выберите действие: ";

                int action;
                cin >> action;

                if (action == 2) {
                    break;
                }
            }
            else {
                cout << "Должно быть зарегистрировано хотя бы 2 транспортных средства" << endl;
            }

            printVehicleMenu(raceType);
            int vehicleChoice;
            cin >> vehicleChoice;

            if (vehicleChoice == 0) {
                if (race.getVehicles().size() >= 2) {
                    break;
                }
                else {
                    cout << "Необходимо зарегистрировать хотя бы 2 транспортных средства!" << endl;
                    system("pause");
                    continue;
                }
            }

            auto vehicle = createVehicle(vehicleChoice);
            if (vehicle) {
                if (race.addVehicle(vehicle)) {
                    cout << vehicle->getName() << " успешно зарегистрирован!" << endl;
                }
                else {
                    cout << "Невозможно зарегистрировать это транспортное средство для данной гонки или оно уже зарегистрировано" << endl;
                }
            }
            else {
                cout << "Неверный выбор транспортного средства" << endl;
            }

            system("pause");
        }

        // Проведение гонки и вывод результатов
        auto results = race.getResults();

        cout << "\nРезультаты гонки:" << endl;
        cout << "-----------------" << endl;
        for (size_t i = 0; i < results.size(); ++i) {
            cout << i + 1 << ". " << results[i].first << ". Время: " << results[i].second << endl;
        }
        cout << "-----------------" << endl;

        cout << "\n1. Провести ещё одну гонку" << endl;
        cout << "2. Выйти" << endl;
        cout << "Выберите действие: ";

        int finalChoice;
        cin >> finalChoice;

        if (finalChoice == 2) {
            break;
        }
    }

    return 0;
}

void printRaceTypeMenu() {
    system("cls");
    cout << "Добро пожаловать в гоночный симулятор!" << endl;
    cout << "1. Гонка для наземного транспорта" << endl;
    cout << "2. Гонка для воздушного транспорта" << endl;
    cout << "3. Гонка для наземного и воздушного транспорта" << endl;
    cout << "0. Выйти" << endl;
    cout << "Выберите тип гонки: ";
}

void printVehicleMenu(RaceType raceType) {
    system("cls");
    cout << "Доступные транспортные средства:" << endl;

    vector<string> groundVehicles = {
        "1. Верблюд",
        "2. Верблюд-быстроход",
        "3. Кентавр",
        "4. Ботинки-вездеходы"
    };

    vector<string> airVehicles = {
        "5. Ковёр-самолёт",
        "6. Орёл",
        "7. Метла"
    };

    if (raceType == RaceType::GROUND) {
        for (const auto& item : groundVehicles) {
            cout << item << endl;
        }
    }
    else if (raceType == RaceType::AIR) {
        for (const auto& item : airVehicles) {
            cout << item << endl;
        }
    }
    else {
        for (const auto& item : groundVehicles) {
            cout << item << endl;
        }
        for (const auto& item : airVehicles) {
            cout << item << endl;
        }
    }

    cout << "0. Закончить регистрацию" << endl;
    cout << "Выберите транспортное средство: ";
}

shared_ptr<Vehicle> createVehicle(int choice) {
    switch (choice) {
    case 1: return make_shared<Camel>();
    case 2: return make_shared<FastCamel>();
    case 3: return make_shared<Centaur>();
    case 4: return make_shared<AllTerrainBoots>();
    case 5: return make_shared<MagicCarpet>();
    case 6: return make_shared<Eagle>();
    case 7: return make_shared<Broom>();
    default: return nullptr;
    }
}

RaceType getRaceTypeFromChoice(int choice) {
    switch (choice) {
    case 1: return RaceType::GROUND;
    case 2: return RaceType::AIR;
    case 3: return RaceType::MIXED;
    default: return RaceType::GROUND;
    }
}

string getRaceTypeName(RaceType type) {
    switch (type) {
    case RaceType::GROUND: return "наземного транспорта";
    case RaceType::AIR: return "воздушного транспорта";
    case RaceType::MIXED: return "всех типов транспорта";
    default: return "";
    }
}
