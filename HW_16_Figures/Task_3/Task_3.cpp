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
        std::cout << name << ": " << std::endl;
    }
    void get_sides_cnt() {
        std::cout << sides_cnt << std::endl;
    }

    virtual bool check() {
        if (sides_cnt == 0) return true;
        else return false;
    }

    virtual void print_info() {
        std::cout << name << ":" << std::endl;
        if (check()) std::cout << "Правильная" << std::endl;
        else std::cout << "Неправильная" << std::endl;
        std::cout << "Количество сторон: " << sides_cnt << std::endl;
    }
};

class Triangle : public Figure {
public:
    int a, b, c, A, B, C;
    Triangle() {
        sides_cnt = 3, a = 0, b = 0, c = 0, A = 0, B = 0, C = 0;
        name = "Треугольник";
    }

    Triangle(int a, int b, int c, int A, int B, int C) {
        sides_cnt = 3;
        name = "Треугольник";
        this->a = a;
        this->b = b;
        this->c = c;
        this->A = A;
        this->B = B;
        this->C = C;
    }

    virtual bool check() override {
        if (sides_cnt == 3 && A+B+C == 180) return true;
        else return false;
    }

    virtual void print_info() override {
        std::cout << std::endl;
        Figure::print_info();
        std::cout << "Стороны: ";
        std::cout << "a=" << a << " ";
        std::cout << "b=" << b << " ";
        std::cout << "c=" << c << std::endl;
        std::cout << "Углы: ";
        std::cout << "A=" << A << " ";
        std::cout << "B=" << B << " ";
        std::cout << "C=" << C << std::endl;
    }
};

class Right_Triangle : public Triangle {
public:
    Right_Triangle() {
        sides_cnt = 3, a = 0, b = 0, c = 0, A = 0, B = 0, C = 90;
        name = "Прямоугольный треугольник";
    }

    Right_Triangle(int a, int b, int c, int A, int B) {
        sides_cnt = 3;
        name = "Прямоугольный треугольник";
        this->a = a;
        this->b = b;
        this->c = c;
        this->A = A;
        this->B = B;
        this->C = 90;
    }

    bool check() override {
        if (Triangle::check() && C == 90) return true;
        else return false;
    }

    void print_info() override {
        Triangle::print_info();
    }

};


class Isosceles_Triangle : public Triangle {
public:
    Isosceles_Triangle() {
        sides_cnt = 3, a = 0, b = 0, c = 0, A = 0, B = 0, C = 0;
        name = "Равнобедренный треугольник";
    }

    Isosceles_Triangle(int a, int b, int c, int A, int B, int C) {
        sides_cnt = 3;
        name = "Равнобедренный треугольник";
        this->a = a;
        this->b = b;
        this->c = c;
        this->A = A;
        this->B = B;
        this->C = C;
    }

    bool check() override {
        if (Triangle::check() && a==c && A==C) return true;
        else return false;
    }

    void print_info() override {
        Triangle::print_info();
    }
};


class Equil_Triangle : public Triangle {
public:
    Equil_Triangle() {
        sides_cnt = 3, a = 0, b = 0, c = 0, A = 60, B = 60, C = 60;
        name = "Равносторонний треугольник";
    }

    Equil_Triangle(int a) {
        sides_cnt = 3;
        name = "Равносторонний треугольник";
        this->a = a;
        this->b = a;
        this->c = a;
        this->A = 60;
        this->B = 60;
        this->C = 60;
    }

    bool check() override {
        if (Triangle::check() && a == b && b == c && A == B && B == C && A ==60) return true;
        else return false;
    }

    void print_info() override {
        Triangle::print_info();
    }
};


class Quad : public Figure {
public:
    int a, b, c, d, A, B, C, D;
    Quad() {
        sides_cnt = 4, a = 0, b = 0, c = 0, d = 0, A = 0, B = 0, C = 0, D = 0;
        name = "Четырехугольник";
    }

    Quad(int a, int b, int c, int d, int A, int B, int C, int D) {
        sides_cnt = 4;
        name = "Четырехугольник";
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
        this->A = A;
        this->B = B;
        this->C = C;
        this->D = D;
    }

    virtual bool check() override {
        if (sides_cnt == 4 && A + B + C + D == 360) return true;
        else return false;
    }

    virtual void print_info() override {
        std::cout << std::endl;
        Figure::print_info();
        std::cout << "Стороны: ";
        std::cout << "a=" << a << " ";
        std::cout << "b=" << b << " ";
        std::cout << "c=" << c << " ";
        std::cout << "d=" << d << std::endl;
        std::cout << "Углы: ";
        std::cout << "A=" << A << " ";
        std::cout << "B=" << B << " ";
        std::cout << "C=" << C << " ";
        std::cout << "D=" << D << std::endl;
    }
};

class Rectangle : public Quad {
public:
    Rectangle() {
        sides_cnt = 4, a = 0, b = 0, c = 0, d = 0, A = 90, B = 90, C = 90, D = 90;
        name = "Прямоугольник";
    }

    Rectangle(int a, int b) {
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

    bool check() override {
        if (Quad::check() && a == c && b == d && A == B && B == C && A == 90) return true;
        else return false;
    }

    void print_info() override {
        Quad::print_info();
    }
};

class Square : public Rectangle {
public:
    Square() {
        sides_cnt = 4, a = 0, b = 0, c = 0, d = 0, A = 90, B = 90, C = 90, D = 90;
        name = "Квадрат";
    }

    Square(int a) {
        sides_cnt = 4;
        name = "Квадрат";
        this->a = a;
        this->b = a;
        this->c = a;
        this->d = a;
        this->A = 90;
        this->B = 90;
        this->C = 90;
        this->D = 90;
    }

    bool check() override {
        if (Quad::check() && a == b && b == c && c == d && A == 90 && B == 90 && C == 90 && D == 90) return true;
        else return false;
    }

    void print_info() override {
        Quad::print_info();
    }
};

class Parallelogram : public Quad {
public:
    Parallelogram() {
        sides_cnt = 4, a = 0, b = 0, c = 0, d = 0, A = 0, B = 0, C = 0, D = 0;
        name = "Параллелограмм";
    }

    Parallelogram(int a, int b, int A, int B) {
        sides_cnt = 4;
        name = "Параллелограмм";
        this->a = a;
        this->b = b;
        this->c = a;
        this->d = b;
        this->A = A;
        this->B = B;
        this->C = A;
        this->D = B;
    }

    bool check() override {
        if (Quad::check() && a == c && b == d && A == C && B == D) return true;
        else return false;
    }

    void print_info() override {
        Quad::print_info();
    }
};

class Romb : public Parallelogram {
public:
    Romb() {
        sides_cnt = 4, a = 0, b = 0, c = 0, d = 0, A = 0, B = 0, C = 0, D = 0;
        name = "Ромб";
    }

    Romb(int a, int A, int B) {
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

    bool check() override {
        if (Quad::check() && a == b && b == c && c == d && A == C && B == D) return true;
        else return false;
    }

    void print_info() override {
        Quad::print_info();
    }

};

int main() {
    setlocale(LC_ALL, "Russian");
    Figure figure;
    figure.print_info(); 

    Triangle triangle(10, 20, 30, 50, 60, 70);
    triangle.print_info();

    Right_Triangle right_triangle(10, 20, 30, 50, 40);
    Triangle* ptr_right_triangle = &right_triangle;
    ptr_right_triangle->print_info();

    Isosceles_Triangle isosceles_triangle(10, 20, 30, 50, 60, 50);
    Triangle* ptr_isosceles_triangle = &isosceles_triangle;
    ptr_isosceles_triangle->print_info();

    Equil_Triangle equil_triangle(30);
    Triangle* ptr_equil_triangle = &equil_triangle;
    ptr_equil_triangle->print_info();

    Quad quad(10, 20, 30, 40, 50, 60, 70, 80);
    quad.print_info();

    Rectangle rectangle(10, 20);
    Quad* ptr_rectangle = &rectangle;
    ptr_rectangle->print_info();

    Square square(20);
    Square* ptr_square = &square;
    ptr_square->print_info();

    Parallelogram parallelogram(20, 30, 30, 40);
    Parallelogram* ptr_parallelogram = &parallelogram;
    ptr_parallelogram->print_info();

    Romb romb(30, 30, 40);
    Romb* ptr_romb = &romb;
    ptr_romb->print_info();

    return 0;
}