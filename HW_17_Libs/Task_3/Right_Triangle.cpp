#include <iostream>
#include "Figure.h"
#include "Triangle.h"
#include "Right_Triangle.h"

Right_Triangle::Right_Triangle() {
        sides_cnt = 3, a = 0, b = 0, c = 0, A = 0, B = 0, C = 90;
        name = "Прямоугольный треугольник";
    }

Right_Triangle::Right_Triangle(int a, int b, int c, int A, int B) {
        sides_cnt = 3;
        name = "Прямоугольный треугольник";
        this->a = a;
        this->b = b;
        this->c = c;
        this->A = A;
        this->B = B;
        this->C = 90;
}