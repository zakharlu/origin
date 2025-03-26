#include <iostream>
#include "Figure.h"
#include "Triangle.h"
#include "Right_Triangle.h"
#include "Isosceles_Triangle.h"
#include "Equil_Triangle.h"
#include "Quad.h"
#include "Rectangle.h"
#include "Square.h"
#include "Parallelogram.h"
#include "Romb.h"

void print_info(Figure* base);

int main()
{
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

    Quad quad(10, 20, 30, 40, 50, 60, 70, 80);
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

}

void print_info(Figure* base) {
    base->get_sides_angles_value();
}