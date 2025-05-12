#include "Centaur.h"

Centaur::Centaur()
    : GroundVehicle("Кентавр", 15.0, 8.0) {}

double Centaur::getRestDuration(int restCount) const {
    return 2.0; // Всегда 2 часа отдыха
}