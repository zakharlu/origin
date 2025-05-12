#pragma once
#include "AirVehicle.h"

class Eagle : public AirVehicle {
public:
    Eagle();
    double getDistanceReduction(double distance) const override;
};

