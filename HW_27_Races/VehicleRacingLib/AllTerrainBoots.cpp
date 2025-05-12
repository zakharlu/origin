#include "AllTerrainBoots.h"

AllTerrainBoots::AllTerrainBoots()
    : GroundVehicle("�������-���������", 6.0, 60.0) {
}

double AllTerrainBoots::getRestDuration(int restCount) const {
    return (restCount == 0) ? 10.0 : 5.0;
}
