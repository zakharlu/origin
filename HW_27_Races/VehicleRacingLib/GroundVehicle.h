#pragma once
#include "Vehicle.h"

class GroundVehicle : public Vehicle {
protected:
    double drivingTime; // ����� �������� �� ������

    virtual double getRestDuration(int restCount) const = 0;  //����� ����������� ������� (=0), ������ ����� �����������, �.�. ��� ����� ��������� ������ ������� ���������� �������� �������
    // restCount - ���������� ��� ��������� ���������

public:
    GroundVehicle(const std::string& name, double speed, double drivingTime); // �������������� ��
    double calculateTime(double distance) const override;
};