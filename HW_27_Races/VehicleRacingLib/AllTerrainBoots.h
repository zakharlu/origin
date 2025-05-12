#pragma once
#include "GroundVehicle.h"

class AllTerrainBoots : public GroundVehicle {
public:
    AllTerrainBoots();
    double getRestDuration(int restCount) const override;
};
