#include "Eagle.h"

Eagle::Eagle()
    : AirVehicle("���", 8.0) {
}

double Eagle::getDistanceReduction(double distance) const {
    return 0.06; // ������ 6%
}
