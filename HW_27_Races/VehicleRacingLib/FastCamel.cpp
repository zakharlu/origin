#include "FastCamel.h"

FastCamel::FastCamel()
    : GroundVehicle("Верблюд-быстроход", 40.0, 10.0) {
}

double FastCamel::getRestDuration(int restCount) const {
    if (restCount == 0) return 5.0;
    if (restCount == 1) return 6.5;
    return 8.0;
}
