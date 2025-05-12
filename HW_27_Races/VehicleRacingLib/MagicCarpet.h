#pragma once
#include "AirVehicle.h"

class MagicCarpet : public AirVehicle {
public:
    MagicCarpet();
    double getDistanceReduction(double distance) const override;
};
