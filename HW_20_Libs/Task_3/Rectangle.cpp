#include <iostream>
#include "Figure.h"
#include "Quad.h"
#include "Rectangle.h"


Rectangle::Rectangle() {
    sides_cnt = 4, a = 0, b = 0, c = 0, d = 0, A = 90, B = 90, C = 90, D = 90;
    name = "Прямоугольник";
}

Rectangle::Rectangle(int a, int b) {
    sides_cnt = 4;
    name = "Прямоугольник";
    this->a = a;
    this->b = b;
    this->c = a;
    this->d = b;
    this->A = 90;
    this->B = 90;
    this->C = 90;
    this->D = 90;
}