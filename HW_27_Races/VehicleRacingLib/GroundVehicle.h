#pragma once
#include "Vehicle.h"

class GroundVehicle : public Vehicle {
protected:
    double drivingTime; // время движения до отдыха

    virtual double getRestDuration(int restCount) const = 0;  //Чисто виртуальная функция (=0), делает класс абстрактным, т.к. нам нужно создавать только объекты конкретных конечных классов
    // restCount - количество уже сделанных остановок

public:
    GroundVehicle(const std::string& name, double speed, double drivingTime); // Инициализируем ТС
    double calculateTime(double distance) const override;
};