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
#include "Exceptions.h"

void print_info(Figure* base);

int main()
{
    setlocale(LC_ALL, "Russian");   
    try {
        Triangle triangle(10, 20, 30, 50, 60, 700);
        Figure* ptr_figure = &triangle;
        print_info(ptr_figure);
    }
    catch (Parameters_Error par_err) {
        std::cout << par_err.what();
    }

    try {
        Right_Triangle right_triangle(15, 25, 35, 40, 500);
        Figure* ptr_right_triangle = &right_triangle;
        print_info(ptr_right_triangle);
    }
    catch (Parameters_Error par_err) {
        std::cout << par_err.what();
    }

    try {
        Isosceles_Triangle isosceles_triangle(10, 20, 50, 60);
        Figure* ptr_isosceles_triangle = &isosceles_triangle;
        print_info(ptr_isosceles_triangle);
    }
    catch (Parameters_Error par_err) {
        std::cout << par_err.what();
    }

    try {
        Equil_Triangle equil_triangle(30);
        Figure* ptr_equil_triangle = &equil_triangle;
        print_info(ptr_equil_triangle);
    }
    catch (Parameters_Error par_err) {
        std::cout << par_err.what();
    }

    try {
        Quad quad(10, 20, 30, 40, 50, 60, 70, 80);
        Figure* ptr_quad = &quad;
        print_info(ptr_quad);
    }
    catch (Parameters_Error par_err) {
        std::cout << par_err.what();
    }

    try {
        Rectangle rectangle(10, 20);
        Figure* ptr_rectangle = &rectangle;
        print_info(ptr_rectangle);
    }
    catch (Parameters_Error par_err) {
        std::cout << par_err.what();
    }

    try {
        Square square(20);
        Figure* ptr_square = &square;
        print_info(ptr_square);
    }
    catch (Parameters_Error par_err) {
        std::cout << par_err.what();
    }

    try {
        Parallelogram parallelogram(20, 30, 30, 40);
        Figure* ptr_parallelogram = &parallelogram;
        print_info(ptr_parallelogram);
    }
    catch (Parameters_Error par_err) {
        std::cout << par_err.what();
    }

    try {
        Romb romb(30, 30, 40);
        Figure* ptr_romb = &romb;
        print_info(ptr_romb);
    }
    catch (Parameters_Error par_err) {
        std::cout << par_err.what();
    }

}

void print_info(Figure* base) {
    base->get_sides_angles_value();
}