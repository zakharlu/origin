#include "Camel.h"

Camel::Camel() : GroundVehicle("Верблюд", 10.0, 30.0) {}

double Camel::getRestDuration(int restCount) const {
    return (restCount == 0) ? 5.0 : 8.0; // Если первый отдых, то счетчик кол-ва отдыхов =0, считыаем время на отдых с учетом этого.
}