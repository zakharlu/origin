#include "transport.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

void printMainMenu() {
    std::cout << "\n�������� ���������" << std::endl;
    std::cout << "1. ����� ��� ��������� ����������" << std::endl;
    std::cout << "2. ����� ��� ���������� ����������" << std::endl;
    std::cout << "3. ����� ��� ��������� � ���������� ����������" << std::endl;
    std::cout << "�������� ��� �����: ";
}

void printTransportList(const std::vector<Transport*>& transports,
    const std::vector<bool>& registered) {
    for (size_t i = 0; i < transports.size(); ++i) {
        std::cout << i + 1 << ". " << transports[i]->getName();
        if (registered[i]) std::cout << " (���������������)";
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

    // ������������ ����������
    for (auto t : raceTransports) {
        results.emplace_back(t->getName(), t->calcTimeRide(distance));
    }

    // ��������� �� �������
    std::sort(results.begin(), results.end(),
        [](const auto& a, const auto& b) { return a.second < b.second; });

    // ������� ����������
    std::cout << "\n=== ���������� ����� ===" << std::endl;
    std::cout << "���������: " << distance << " ��" << std::endl;
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << i + 1 << ". " << results[i].first
            << ": " << results[i].second << " �" << std::endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    // ������������� ����������
    size_t totalTransports = 0;
    Transport** allTransports = makeTransports(totalTransports);
    std::vector<Transport*> transportsList(allTransports, allTransports + totalTransports);

    while (true) {
        // ����� ���� �����
        TransportType raceType;
        while (true) {
            printMainMenu();
            int typeChoice;
            std::cin >> typeChoice;

            if (typeChoice == 1) { raceType = TransportType::GROUND; break; }
            if (typeChoice == 2) { raceType = TransportType::AIR; break; }
            if (typeChoice == 3) { raceType = TransportType::MIXED; break; }
        }

        // ���� ���������
        int distance;
        std::cout << "\n ������� ����� ��������� (������ ���� ������������): ";
        std::cin >> distance;
        while (distance <= 0) {
            std::cout << "��������� ������ ���� �������������. ��������� ����: ";
            std::cin >> distance;
        }

        // ����������� ����������
        std::vector<Transport*> raceTransports;
        std::vector<bool> registered(totalTransports, false);

        while (true) {
            system("cls");
            std::cout << std::endl;
            std::cout << (raceType == TransportType::GROUND ? "����� ��� ��������� ����������" :
                    raceType == TransportType::AIR ? "����� ��� ���������� ����������" : "����� ��� ���������� ����������")
                << std::endl;
            std::cout << "����������: " << distance << " ��" << std::endl;
            std::cout << "\n��������� ������������ ��������:" << std::endl;

            printTransportList(transportsList, registered);

            std::cout << "0. ��������� �����������. ������ �����" << std::endl;
            std::cout << "�������� ��������� ��� 0 ��� ��������� �������� ����������� � ������ �����: ";

            int transportChoice;
            std::cin >> transportChoice;

            if (transportChoice == 0) {
                if (raceTransports.size() < 2) {
                    std::cout << "���������� ���������������� ������� 2 ������������ ��������!" << std::endl;
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
                std::cout << "��� ������������ �������� ��� ����������������!" << std::endl;
                system("pause");
                continue;
            }

            if (!isValidForRace(selected, raceType)) {
                std::cout << "���� ��������� �� ����� ����������� � ������ �����!" << std::endl;
                system("pause");
                continue;
            }

            registered[index] = true;
            raceTransports.push_back(selected);
            std::cout << selected->getName() << " ������� ���������������!" << std::endl;
            system("pause");
        }

        int choice;

        // ���������� �����
        conductRace(raceTransports, distance);
        system("pause");

        // ����������� ����������
        std::cout << "\n1. �������� ��� ���� �����" << std::endl;
        std::cout << "2. �����" << std::endl;
        std::cout << "�������� ��������: ";
        std::cin >> choice;
        if (choice == 2) break;
    }

    // ������� ������
    for (size_t i = 0; i < totalTransports; ++i) {
        delete allTransports[i];
    }
    delete[] allTransports;

    return 0;
}