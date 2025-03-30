#include <iostream>
#include "Figure.h"
#include "Quad.h"
#include "Rectangle.h"
#include "Parallelogram.h"

Parallelogram:: Parallelogram() {
    sides_cnt = 4, a = 0, b = 0, c = 0, d = 0, A = 0, B = 0, C = 0, D = 0;
    name = "ֿאנאככוכמדנאלל";
}

Parallelogram::Parallelogram(int a, int b, int A, int B) {
    sides_cnt = 4;
    name = "ֿאנאככוכמדנאלל";
    this->a = a;
    this->b = b;
    this->c = a;
    this->d = b;
    this->A = A;
    this->B = B;
    this->C = A;
    this->D = B;
}