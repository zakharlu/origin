#include "Eagle.h"

Eagle::Eagle()
    : AirVehicle("Орёл", 8.0) {
}

double Eagle::getDistanceReduction(double distance) const {
    return 0.06; // Всегда 6%
}
