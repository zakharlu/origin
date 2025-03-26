#include <math.h>
#include "maths.h"

int add(int a, int b) {
	return a + b;
}
int subtract(int a, int b) {
	return a - b;
}
int mult(int a, int b) {
	return a * b;
}
double division(int a, int b) {
	return static_cast<double>(a) / static_cast<double>(b);
}
int sqrt(int a, int b) {
	return pow(a, b);
}