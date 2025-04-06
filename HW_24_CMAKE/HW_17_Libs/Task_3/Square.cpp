#include <iostream>
#include "Figure.h"
#include "Quad.h"
#include "Rectangle.h"
#include "Square.h"

Square::Square() {
    sides_cnt = 4, a = 0, b = 0, c = 0, d = 0, A = 90, B = 90, C = 90, D = 90;
    name = " вадрат";
}

Square::Square(int a) {
    sides_cnt = 4;
    name = " вадрат";
    this->a = a;
    this->b = a;
    this->c = a;
    this->d = a;
    this->A = 90;
    this->B = 90;
    this->C = 90;
    this->D = 90;
}