#include "MagicCarpet.h"

MagicCarpet::MagicCarpet()
    : AirVehicle("Ковёр-самолёт", 10.0) {
}

double MagicCarpet::getDistanceReduction(double distance) const {
    if (distance < 1000) return 0.0;
    if (distance < 5000) return 0.03;
    if (distance < 10000) return 0.1;
    return 0.05;
}
