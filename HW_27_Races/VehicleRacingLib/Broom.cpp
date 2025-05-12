#include "Broom.h"
#include <cmath>

Broom::Broom()
    : AirVehicle("�����", 20.0) {
}

double Broom::getDistanceReduction(double distance) const {
    double reductionPercent = std::floor(distance / 1000) * 0.01;
    return std::min(reductionPercent, 1.0); // �� ����� 100%
}
