#include <iostream>
#include "Figure.h"
#include "Triangle.h"
#include "Isosceles_Triangle.h"

Isosceles_Triangle::Isosceles_Triangle() {
    sides_cnt = 3, a = 0, b = 0, c = 0, A = 0, B = 0, C = 0;
    name = "Равнобедренный треугольник";
}

Isosceles_Triangle::Isosceles_Triangle(int a, int b, int A, int B) : Triangle(a, b, a, A, B, A) {
    sides_cnt = 3;
    name = "Равнобедренный треугольник";
    this->a = a;
    this->b = b;
    this->c = a;
    this->A = A;
    this->B = B;
    this->C = A;
}
