#pragma once
#include "GroundVehicle.h"

class Centaur : public GroundVehicle {
public:
    Centaur();
    double getRestDuration(int restCount) const override;
};

