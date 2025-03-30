#include <iostream>
#include "Figure.h"
#include "Triangle.h"
#include "Isosceles_Triangle.h"

Isosceles_Triangle::Isosceles_Triangle() {
    sides_cnt = 3, a = 0, b = 0, c = 0, A = 0, B = 0, C = 0;
    name = "�������������� �����������";
}

Isosceles_Triangle::Isosceles_Triangle(int a, int b, int c, int A, int B, int C) {
    if (a == c && A == C) {
        sides_cnt = 3;
        name = "�������������� �����������";
        this->a = a;
        this->b = b;
        this->c = c;
        this->A = A;
        this->B = B;
        this->C = C;
    }
    else {
        std::cout << "������ ������������� ��������������� ������������" << std::endl;
    }
}
