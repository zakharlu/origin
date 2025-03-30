#include <iostream>
#include "Figure.h"
#include "Quad.h"

Quad::Quad() {
    sides_cnt = 4, a = 0, b = 0, c = 0, d = 0, A = 0, B = 0, C = 0, D = 0;
    name = "Четырехугольник";
}

Quad::Quad(int a, int b, int c, int d, int A, int B, int C, int D) {
    sides_cnt = 4;
    name = "Четырехугольник";
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
    std::cout << "Стороны: ";
    std::cout << "a=" << a << " ";
    std::cout << "b=" << b << " ";
    std::cout << "c=" << c << " ";
    std::cout << "d=" << d << " " << std::endl;
    std::cout << "Углы: ";
    std::cout << "A=" << A << " ";
    std::cout << "B=" << B << " ";
    std::cout << "C=" << C << " ";
    std::cout << "D=" << D << std::endl << std::endl;
}