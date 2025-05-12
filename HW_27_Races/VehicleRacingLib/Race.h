#pragma once
#include <vector>
#include <memory>
#include "Vehicle.h"

enum class RaceType {
    GROUND,
    AIR,
    MIXED
};

class Race {
private:
    RaceType type;
    double distance;
    std::vector<std::shared_ptr<Vehicle>> vehicles; // ������������ ������ ����� ���������� �� ������� ���� �������� ������ Vehicle

public:
    Race(RaceType type, double distance);

    bool addVehicle(const std::shared_ptr<Vehicle>& vehicle); // ������� �������� ��, ��������� ����������� ������ �� ����� ��������� shared_ptr  
    std::vector<std::pair<std::string, double>> getResults() const; // ���������� ���� - �������� ��, ������������ ����� ����������� ����������

    RaceType getType() const;
    double getDistance() const;
    const std::vector<std::shared_ptr<Vehicle>>& getVehicles() const; //������������ ����������� ������ �� ������, ���������� shared_ptr.
};