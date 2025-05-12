#pragma once
#include <string>

enum class VehicleType { // Создаем класс перечислений для типов ТС
    GROUND,
    AIR
};

class Vehicle { // Объявляем базовый класс для ТС
protected:
    std::string name;
    double speed;
    VehicleType type;

public:
    Vehicle(const std::string& name, double speed, VehicleType type); // Конструктор для класса Vehicle
    virtual ~Vehicle() {}; // Виртуальный деструктор для удаления в дальнейшем производных объектов через указатель на базовый класс 

    virtual double calculateTime(double distance) const = 0; // Делаем класс абстрактным (=0), т.к. нужно создавать только объекты конкретных классов, а не базового

    std::string getName() const;
    double getSpeed() const;
    VehicleType getType() const;
};
