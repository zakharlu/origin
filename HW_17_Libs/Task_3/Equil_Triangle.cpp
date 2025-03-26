#include <iostream>
#include "Figure.h"
#include "Triangle.h"
#include "Equil_Triangle.h"

Equil_Triangle::Equil_Triangle() {
    sides_cnt = 3, a = 0, b = 0, c = 0, A = 60, B = 60, C = 60;
    name = "Равносторонний треугольник";
}

Equil_Triangle::Equil_Triangle(int a) {
    sides_cnt = 3;
    name = "Равносторонний треугольник";
    this->a = a;
    this->b = a;
    this->c = a;
    this->A = 60;
    this->B = 60;
    this->C = 60;
}