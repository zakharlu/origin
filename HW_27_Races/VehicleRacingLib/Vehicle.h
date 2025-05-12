#pragma once
#include <string>

enum class VehicleType { // ������� ����� ������������ ��� ����� ��
    GROUND,
    AIR
};

class Vehicle { // ��������� ������� ����� ��� ��
protected:
    std::string name;
    double speed;
    VehicleType type;

public:
    Vehicle(const std::string& name, double speed, VehicleType type); // ����������� ��� ������ Vehicle
    virtual ~Vehicle() {}; // ����������� ���������� ��� �������� � ���������� ����������� �������� ����� ��������� �� ������� ����� 

    virtual double calculateTime(double distance) const = 0; // ������ ����� ����������� (=0), �.�. ����� ��������� ������ ������� ���������� �������, � �� ��������

    std::string getName() const;
    double getSpeed() const;
    VehicleType getType() const;
};
