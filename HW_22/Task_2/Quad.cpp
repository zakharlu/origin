#include <iostream>
#include "Figure.h"
#include "Quad.h"
#include "Exceptions.h"

Quad::Quad() {
    sides_cnt = 4, a = 0, b = 0, c = 0, d = 0, A = 0, B = 0, C = 0, D = 0;
    name = "���������������";
}

Quad::Quad(int a, int b, int c, int d, int A, int B, int C, int D) {
    if (A + B + C +D != 360) {
        throw Parameters_Error("������ �������� ������. �������: ����� ����� �� ����� 360\n");
    }
    sides_cnt = 4;
    name = "���������������";
    this->a = a;
    this->b = b;
    this->c = c;
    this->d = d;
    this->A = A;
    this->B = B;
    this->C = C;
    this->D = D;
}

void Quad::get_sides_angles_value() {
    Figure::get_sides_angles_value();
    std::cout << name << "(������� " << a << ", " << b << ", " << c << ", " << d << "; ���� " << A << ", " << B << ", " << C << ", " << D << ") ������" << std::endl;
}