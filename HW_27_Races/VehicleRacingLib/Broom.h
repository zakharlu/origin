#pragma once
#include "AirVehicle.h"

class Broom : public AirVehicle {
public:
    Broom();
    double getDistanceReduction(double distance) const override;
};
