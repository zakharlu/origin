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
        // ����� ���� �����
        printRaceTypeMenu();
        int raceTypeChoice;
        cin >> raceTypeChoice;

        if (raceTypeChoice == 0) {
            break;
        }

        RaceType raceType = getRaceTypeFromChoice(raceTypeChoice);

        // ���� ���������
        double distance;
        cout << "������� ����� ��������� (������ ���� ������������): ";
        cin >> distance;
        while (distance <= 0) {
            cout << "��������� ������ ���� �������������. ���������� �����: ";
            cin >> distance;
        }

        // �������� �����
        Race race(raceType, distance);

        // ����������� ������������ �������
        while (true) {
            system("cls");
            cout << "����� ��� " << getRaceTypeName(raceType) << ". ���������: " << distance << endl;

            if (race.getVehicles().size() >= 2) {
                cout << "������������������ ������������ ��������:" << endl;
                for (const auto& vehicle : race.getVehicles()) {
                    cout << " - " << vehicle->getName() << endl;
                }
                cout << "\n1. ���������������� ���������" << endl;
                cout << "2. ������ �����" << endl;
                cout << "�������� ��������: ";

                int action;
                cin >> action;

                if (action == 2) {
                    break;
                }
            }
            else {
                cout << "������ ���� ���������������� ���� �� 2 ������������ ��������" << endl;
            }

            printVehicleMenu(raceType);
            int vehicleChoice;
            cin >> vehicleChoice;

            if (vehicleChoice == 0) {
                if (race.getVehicles().size() >= 2) {
                    break;
                }
                else {
                    cout << "���������� ���������������� ���� �� 2 ������������ ��������!" << endl;
                    system("pause");
                    continue;
                }
            }

            auto vehicle = createVehicle(vehicleChoice);
            if (vehicle) {
                if (race.addVehicle(vehicle)) {
                    cout << vehicle->getName() << " ������� ���������������!" << endl;
                }
                else {
                    cout << "���������� ���������������� ��� ������������ �������� ��� ������ ����� ��� ��� ��� ����������������" << endl;
                }
            }
            else {
                cout << "�������� ����� ������������� ��������" << endl;
            }

            system("pause");
        }

        // ���������� ����� � ����� �����������
        auto results = race.getResults();

        cout << "\n���������� �����:" << endl;
        cout << "-----------------" << endl;
        for (size_t i = 0; i < results.size(); ++i) {
            cout << i + 1 << ". " << results[i].first << ". �����: " << results[i].second << endl;
        }
        cout << "-----------------" << endl;

        cout << "\n1. �������� ��� ���� �����" << endl;
        cout << "2. �����" << endl;
        cout << "�������� ��������: ";

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
    cout << "����� ���������� � �������� ���������!" << endl;
    cout << "1. ����� ��� ��������� ����������" << endl;
    cout << "2. ����� ��� ���������� ����������" << endl;
    cout << "3. ����� ��� ��������� � ���������� ����������" << endl;
    cout << "0. �����" << endl;
    cout << "�������� ��� �����: ";
}

void printVehicleMenu(RaceType raceType) {
    system("cls");
    cout << "��������� ������������ ��������:" << endl;

    vector<string> groundVehicles = {
        "1. �������",
        "2. �������-���������",
        "3. �������",
        "4. �������-���������"
    };

    vector<string> airVehicles = {
        "5. ����-������",
        "6. ���",
        "7. �����"
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

    cout << "0. ��������� �����������" << endl;
    cout << "�������� ������������ ��������: ";
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
    case RaceType::GROUND: return "��������� ����������";
    case RaceType::AIR: return "���������� ����������";
    case RaceType::MIXED: return "���� ����� ����������";
    default: return "";
    }
}
