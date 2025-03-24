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
    
    virtual void get_sides_angles_value() {
        std::cout << name << ":" << std::endl;
    }
};

class Triangle: public Figure {
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

    void get_sides_angles_value() override {
        Figure::get_sides_angles_value();
        std::cout << "Стороны: ";
        std::cout << "a=" << a << " ";
        std::cout << "b=" << b << " ";
        std::cout << "c=" << c << " " << std::endl;
        std::cout << "Углы: ";
        std::cout << "A=" << A << " ";
        std::cout << "B=" << B << " ";
        std::cout << "C=" << C << std::endl << std::endl;
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
};


class Isosceles_Triangle : public Triangle {
public:
    Isosceles_Triangle() {
        sides_cnt = 3, a = 0, b = 0, c = 0, A = 0, B = 0, C = 0;
        name = "Равнобедренный треугольник";
    }

    Isosceles_Triangle(int a, int b, int c, int A, int B, int C) {
        if (a == c && A == C) {
            sides_cnt = 3;
            name = "Равнобедренный треугольник";
            this->a = a;
            this->b = b;
            this->c = c;
            this->A = A;
            this->B = B;
            this->C = C;
        }
        else {
            std::cout << "Ошибка инициализации равнобедренного треугольника" << std::endl;
        }
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

    void get_sides_angles_value() override {
        Figure::get_sides_angles_value();
        std::cout << "Стороны: ";
        std::cout << "a=" << a << " ";
        std::cout << "b=" << b << " ";
        std::cout << "c=" << c << " ";
        std::cout << "d=" << d << " " << std::endl;
        std::cout << "Углы: ";
        std::cout << "A=" << A << " ";
        std::cout << "B=" << B << " ";
        std::cout << "C=" << C << " ";
        std::cout << "D=" << D << std::endl << std::endl;
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
};


void print_info(Figure* base);

int main() {
    setlocale(LC_ALL, "Russian");
    Triangle triangle(10, 20, 30, 50, 60, 70);
    Figure* ptr_figure = &triangle;
    print_info(ptr_figure);

    Right_Triangle right_triangle(10, 20, 30, 50, 60);
    Figure* ptr_right_triangle = &right_triangle;
    print_info(ptr_right_triangle);

    Isosceles_Triangle isosceles_triangle(10, 20, 10, 50, 60, 50);
    Figure* ptr_isosceles_triangle = &isosceles_triangle;
    print_info(ptr_isosceles_triangle);

    Equil_Triangle equil_triangle(30);
    Figure* ptr_equil_triangle = &equil_triangle;
    print_info(ptr_equil_triangle);

    Quad quad(10, 20, 30, 40, 50, 60, 70 ,80);
    Figure* ptr_quad = &quad;
    print_info(ptr_quad);

    Rectangle rectangle(10, 20);
    Figure* ptr_rectangle = &rectangle;
    print_info(ptr_rectangle);

    Square square(20);
    Figure* ptr_square = &square;
    print_info(ptr_square);

    Parallelogram parallelogram(20, 30, 30, 40);
    Figure* ptr_parallelogram = &parallelogram;
    print_info(ptr_parallelogram);

    Romb romb(30, 30, 40);
    Figure* ptr_romb = &romb;
    print_info(ptr_romb);

   return 0;
}


void print_info(Figure* base) {
    base->get_sides_angles_value();
}