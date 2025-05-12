#pragma once
#include "GroundVehicle.h"

class Camel : public GroundVehicle {
public:
    Camel();
    double getRestDuration(int restCount) const override;
};
