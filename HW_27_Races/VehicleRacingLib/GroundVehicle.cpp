#include "GroundVehicle.h"
#include <cmath>

GroundVehicle::GroundVehicle(const std::string& name, double speed, double drivingTime)
    : Vehicle(name, speed, VehicleType::GROUND), drivingTime(drivingTime) { //Инициализируем поля переданными значениями name, speed, GROUND - т.к. наземное ТС, drivingTime - время движения до отдыха
}

double GroundVehicle::calculateTime(double distance) const {
    double totalTime = 0.0;
    double remainingDistance = distance; // оставшееся расстояние = общее расстояние
    int restCount = 0;

    while (remainingDistance > 0) {  
        double distanceBeforeRest = speed * drivingTime; // Рассчитываем расстояние до отдыха

        if (remainingDistance <= distanceBeforeRest) { // оставшееся расстояние меньше расстояния до отдыха. 
            totalTime += remainingDistance / speed;    // Рассчитываем время на прохождение оставшегося отрезка и прибавляем к общему времени
            break;
        }

        totalTime += drivingTime; // К общему времени прибавляем время движения до отдыха
        remainingDistance -= distanceBeforeRest; // сокращаем оставшееся расстояние на расстояние до отдыха
        totalTime += getRestDuration(restCount); //  Прибавляем к общему времени время для отдыха (для каждого производного класса объекта свое)
        restCount++; // Увеличиваем счетчик количества отдыхов
    }

    return totalTime;
}