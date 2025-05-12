#include "GroundVehicle.h"
#include <cmath>

GroundVehicle::GroundVehicle(const std::string& name, double speed, double drivingTime)
    : Vehicle(name, speed, VehicleType::GROUND), drivingTime(drivingTime) { //�������������� ���� ����������� ���������� name, speed, GROUND - �.�. �������� ��, drivingTime - ����� �������� �� ������
}

double GroundVehicle::calculateTime(double distance) const {
    double totalTime = 0.0;
    double remainingDistance = distance; // ���������� ���������� = ����� ����������
    int restCount = 0;

    while (remainingDistance > 0) {  
        double distanceBeforeRest = speed * drivingTime; // ������������ ���������� �� ������

        if (remainingDistance <= distanceBeforeRest) { // ���������� ���������� ������ ���������� �� ������. 
            totalTime += remainingDistance / speed;    // ������������ ����� �� ����������� ����������� ������� � ���������� � ������ �������
            break;
        }

        totalTime += drivingTime; // � ������ ������� ���������� ����� �������� �� ������
        remainingDistance -= distanceBeforeRest; // ��������� ���������� ���������� �� ���������� �� ������
        totalTime += getRestDuration(restCount); //  ���������� � ������ ������� ����� ��� ������ (��� ������� ������������ ������ ������� ����)
        restCount++; // ����������� ������� ���������� �������
    }

    return totalTime;
}