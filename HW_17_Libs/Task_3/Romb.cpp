#include <iostream>
#include "Figure.h"
#include "Quad.h"
#include "Parallelogram.h"
#include "Romb.h"

Romb::Romb() {
    sides_cnt = 4, a = 0, b = 0, c = 0, d = 0, A = 0, B = 0, C = 0, D = 0;
    name = "Ромб";
}

Romb::Romb(int a, int A, int B) {
    sides_cnt = 4;
    name = "Ромб";
    this->a = a;
    this->b = a;
    this->c = a;
    this->d = a;
    this->A = A;
    this->B = B;
    this->C = A;
    this->D = B;
}