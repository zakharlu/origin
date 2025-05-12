#include "AirVehicle.h"

AirVehicle::AirVehicle(const std::string& name, double speed)
    : Vehicle(name, speed, VehicleType::AIR) {
}

double AirVehicle::calculateTime(double distance) const {
    double reducedDistance = distance * (1.0 - getDistanceReduction(distance)); // Рассчитываем сокращенное расстояние воздушного ТС 
    return reducedDistance / speed; // Возвращаем время на прохождение сокращенного расстояния
}