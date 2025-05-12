#include "Camel.h"

Camel::Camel() : GroundVehicle("�������", 10.0, 30.0) {}

double Camel::getRestDuration(int restCount) const {
    return (restCount == 0) ? 5.0 : 8.0; // ���� ������ �����, �� ������� ���-�� ������� =0, �������� ����� �� ����� � ������ �����.
}