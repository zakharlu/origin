#include <iostream>

class Figure {
protected:
    int sides_cnt;
    std::string name;

public:
    Figure() {
        sides_cnt = 0;
        name = "Фигура";
    }
    void get_name() {
        std::cout << name << ": ";
    }
    void get_sides_cnt() {
        std::cout << sides_cnt << std::endl;
    }
};

class Triangle: public Figure {
public:
    Triangle() {
        sides_cnt = 3;
        name = "Треугольник";
    }
};

class Quad: public Figure {
public:
    Quad() {
        sides_cnt = 4;
        name = "Четырехугольник";
    }
};


int main() {
    setlocale(LC_ALL, "Russian");
    Figure figure;
    figure.get_name();
    figure.get_sides_cnt();

    Triangle triangle;
    triangle.get_name();
    triangle.get_sides_cnt();

    Quad quad;
    quad.get_name();
    quad.get_sides_cnt();

    return 0;
}