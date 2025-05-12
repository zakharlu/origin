#pragma once
#include <vector>
#include <memory>
#include "Vehicle.h"

enum class RaceType {
    GROUND,
    AIR,
    MIXED
};

class Race {
private:
    RaceType type;
    double distance;
    std::vector<std::shared_ptr<Vehicle>> vehicles; // Динамический массив умных указателей на объекты типа базового класса Vehicle

public:
    Race(RaceType type, double distance);

    bool addVehicle(const std::shared_ptr<Vehicle>& vehicle); // Попытка добавить ТС, принимает константную ссылку на умный указатель shared_ptr  
    std::vector<std::pair<std::string, double>> getResults() const; // Возвращает пару - название ТС, рассчитанное время прохождения расстояния

    RaceType getType() const;
    double getDistance() const;
    const std::vector<std::shared_ptr<Vehicle>>& getVehicles() const; //Возвращается константная ссылка на вектор, содержащий shared_ptr.
};