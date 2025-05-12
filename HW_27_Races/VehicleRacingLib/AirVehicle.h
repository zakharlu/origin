#pragma once
#include "Vehicle.h"

class AirVehicle : public Vehicle {
protected:
    virtual double getDistanceReduction(double distance) const = 0;

public:
    AirVehicle(const std::string& name, double speed);
    double calculateTime(double distance) const override;
};