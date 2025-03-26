#pragma once
class Quad : public Figure {
public:
    int a, b, c, d, A, B, C, D;
    Quad();
    Quad(int a, int b, int c, int d, int A, int B, int C, int D);
    void get_sides_angles_value() override;
};