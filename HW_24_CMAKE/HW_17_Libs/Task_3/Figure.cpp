#include <iostream>
#include "Figure.h"
#include "Triangle.h"

Figure::Figure() {
	sides_cnt = 0;
	name = "������";
}

void Figure::get_sides_angles_value() {
	std::cout << name << ":" << std::endl;
}

