#include "Centaur.h"

Centaur::Centaur()
    : GroundVehicle("�������", 15.0, 8.0) {}

double Centaur::getRestDuration(int restCount) const {
    return 2.0; // ������ 2 ���� ������
}