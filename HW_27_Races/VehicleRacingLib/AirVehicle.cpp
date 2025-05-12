#include "AirVehicle.h"

AirVehicle::AirVehicle(const std::string& name, double speed)
    : Vehicle(name, speed, VehicleType::AIR) {
}

double AirVehicle::calculateTime(double distance) const {
    double reducedDistance = distance * (1.0 - getDistanceReduction(distance)); // ������������ ����������� ���������� ���������� �� 
    return reducedDistance / speed; // ���������� ����� �� ����������� ������������ ����������
}