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

    void get_sides_angles_value() {
        std::cout << std::endl;
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

    void get_sides_angles_value() {
        std::cout << std::endl;
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


void print_info_3(Triangle* ptr_triangle);
void print_info_4(Quad* ptr_quad);

int main() {
    setlocale(LC_ALL, "Russian");
    Triangle triangle(10, 20, 30, 50, 60, 70);
    triangle.get_name();
    triangle.get_sides_angles_value();


    Right_Triangle right_triangle(10, 20, 30, 50, 60);
    Triangle* ptr_right_triangle = &right_triangle;
    print_info_3(ptr_right_triangle);

    Isosceles_Triangle isosceles_triangle(10, 20, 10, 50, 60, 50);
    Triangle* ptr_isosceles_triangle = &isosceles_triangle;
    print_info_3(ptr_isosceles_triangle);

    Equil_Triangle equil_triangle(30);
    Triangle* ptr_equil_triangle = &equil_triangle;
    print_info_3(ptr_equil_triangle);

   Quad quad(10, 20, 30, 40, 50, 60, 70 ,80);
   quad.get_name();
   quad.get_sides_angles_value();

   Rectangle rectangle(10, 20);
   Quad* ptr_rectangle = &rectangle;
   print_info_4(ptr_rectangle);

   Square square(20);
   Square* ptr_square = &square;
   print_info_4(ptr_square);

   Parallelogram parallelogram(20, 30, 30, 40);
   Parallelogram* ptr_parallelogram = &parallelogram;
   print_info_4(ptr_parallelogram);

   Romb romb(30, 30, 40);
   Romb* ptr_romb = &romb;
   print_info_4(ptr_romb);
   
   return 0;
}

void print_info_3(Triangle* ptr_triangle) {
    ptr_triangle->get_name();
    ptr_triangle->get_sides_angles_value();
}

void print_info_4(Quad* ptr_quad) {
    ptr_quad->get_name();
    ptr_quad->get_sides_angles_value();
}