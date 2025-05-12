#pragma once
#include "GroundVehicle.h"

class FastCamel : public GroundVehicle {
public:
    FastCamel();
    double getRestDuration(int restCount) const override;
};
