#include "Race.h"
#include <algorithm>

Race::Race(RaceType type, double distance) : type(type), distance(distance) {
}

bool Race::addVehicle(const std::shared_ptr<Vehicle>& vehicle) {
    // �������� �� ������������ ���� ����� � ������������� ��������
    if ((type == RaceType::GROUND && vehicle->getType() != VehicleType::GROUND) ||
        (type == RaceType::AIR && vehicle->getType() != VehicleType::AIR)) {
        return false;
    }

    // �������� �� ������������ ������������� ��������
 //   for (const auto& v : vehicles) {
    for (auto it = vehicles.begin(); it != vehicles.end(); ++it) {
        const auto& v = *it;
        if (v->getName() == vehicle->getName()) {
            return false;
        }
    }

    vehicles.push_back(vehicle); // ��������� ������ shared_ptr � ������
    return true;
}

std::vector<std::pair<std::string, double>> Race::getResults() const {
    std::vector<std::pair<std::string, double>> results;

    //for (const auto& vehicle : vehicles) {
    for (auto it = vehicles.begin(); it != vehicles.end(); ++it) { // �������� ������� ��� results
        const auto& vehicle = *it;
        results.emplace_back(vehicle->getName(), vehicle->calculateTime(distance)); // �������� ���� ������������ ��, ����� ����������� ���������
    }

    // ���������� �� ������� (�� �������� � ��������)
    std::sort(results.begin(), results.end(), [](const auto& a, const auto& b) { return a.second < b.second; }); // ���������� �� ���������� Algorithms ������� results �� ����������� ������� (������ �������� ���)

    return results;
}

RaceType Race::getType() const {
    return type;
}

double Race::getDistance() const {
    return distance;
}

const std::vector<std::shared_ptr<Vehicle>>& Race::getVehicles() const {
    return vehicles;
}