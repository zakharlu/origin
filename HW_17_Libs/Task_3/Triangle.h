#pragma once

class Triangle : public Figure {
public:
    int a, b, c, A, B, C;
    Triangle();
    Triangle(int a, int b, int c, int A, int B, int C);
    void get_sides_angles_value() override;
};