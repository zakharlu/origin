#include <iostream>
#include "Figure.h"
#include "Triangle.h"


Triangle::Triangle() {
    sides_cnt = 3, a = 0, b = 0, c = 0, A = 0, B = 0, C = 0;
    name = "�����������";
}

Triangle::Triangle(int a, int b, int c, int A, int B, int C) {
    sides_cnt = 3;
    name = "�����������";
    this->a = a;
    this->b = b;
    this->c = c;
    this->A = A;
    this->B = B;
    this->C = C;
}

void Triangle::get_sides_angles_value() {
    Figure::get_sides_angles_value();
    std::cout << "�������: ";
    std::cout << "a=" << a << " ";
    std::cout << "b=" << b << " ";
    std::cout << "c=" << c << " " << std::endl;
    std::cout << "����: ";
    std::cout << "A=" << A << " ";
    std::cout << "B=" << B << " ";
    std::cout << "C=" << C << std::endl << std::endl;
}