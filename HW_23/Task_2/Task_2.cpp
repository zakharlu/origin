#include <iostream>
#define EPS 0.0000000001

class Fraction
{
private:
	int numerator_;
	int denominator_;

public:
	Fraction(int numerator, int denominator)
	{
		numerator_ = numerator;
		denominator_ = denominator;
	}


	bool operator==(Fraction other) {
		if (((static_cast<double>(numerator_) / static_cast<double>(denominator_)) - (static_cast<double>(other.numerator_) / static_cast<double>(other.denominator_)) < EPS) &&
			((static_cast<double>(numerator_) / static_cast<double>(denominator_)) - (static_cast<double>(other.numerator_) / static_cast<double>(other.denominator_)) > (-EPS))) {
			return true;
		}
		else {
			return false;
		}
	}

	bool operator!=(Fraction other) { return !(*this == other); }

	bool operator<(Fraction other) {
		if (((static_cast<double>(numerator_) / static_cast<double>(denominator_)) - (static_cast<double>(other.numerator_) / static_cast<double>(other.denominator_)) < EPS) &&
			((static_cast<double>(numerator_) / static_cast<double>(denominator_)) - (static_cast<double>(other.numerator_) / static_cast<double>(other.denominator_)) < (-EPS))) {
			return true;
		}
		else {
			return false;
		}
	}

	bool operator>(Fraction other) { return (other < *this); }
	bool operator<=(Fraction other) { return !(*this > other); }
	bool operator>=(Fraction other) { return !(*this < other); }

	Fraction operator+(Fraction other) {
		int numerator_result = numerator_ * other.denominator_ + other.numerator_ * denominator_;
		int denominator_result = denominator_ * other.denominator_;
		int nod = NOD(numerator_result, denominator_result);
		return Fraction(numerator_result / nod, denominator_result / nod);
	}

	Fraction operator-(Fraction other) {
		int numerator_result = numerator_ * other.denominator_ - other.numerator_ * denominator_;
		int denominator_result = denominator_ * other.denominator_;
		int nod = NOD(numerator_result, denominator_result);
		return Fraction(numerator_result / nod, denominator_result / nod);
	}

	Fraction operator*(Fraction other) {
		int numerator_result = numerator_ * other.numerator_;
		int denominator_result = denominator_ * other.denominator_;
		int nod = NOD(numerator_result, denominator_result);
		return Fraction(numerator_result / nod, denominator_result / nod);
	}

	Fraction operator/(Fraction other) {
		int numerator_result = numerator_ * other.denominator_; 
		int denominator_result = denominator_ * other.numerator_;
		int nod = NOD(numerator_result, denominator_result);
		return Fraction(numerator_result / nod, denominator_result / nod);
	}

	Fraction& operator-() {
		numerator_ = (-1) * numerator_;
		return *this;
	}

	Fraction& operator++() {
		numerator_ = numerator_ + denominator_;
		return *this;
	}

	Fraction operator++(int)
	{
		Fraction temp = *this;
		++(*this);
		return temp;
	}

	Fraction& operator--() {
		numerator_ = numerator_ - denominator_;
		return *this;
	}

	Fraction operator--(int)
	{
		Fraction temp = *this;
		--(*this);
		return temp;
	}

	int NOD(int num_1, int num_2) {
		if (num_1 < 0) (num_1 = (-1) * num_1);
		if (num_2 < 0) (num_2 = (-1) * num_2);
		while (num_2 != 0) {
			int tmp = num_2;
			num_2 = num_1 % num_2;
			num_1 = tmp;
		}

		return num_1;
	}

	void Print() { std::cout << numerator_ << "/" << denominator_; }
};

void print(Fraction& f1, Fraction& f2, Fraction& Result, char operator_type);

int main()
{
	int numerator_1 = 0, denominator_1 = 0, numerator_2 = 0, denominator_2 = 0;
	setlocale(LC_ALL, "Russian");
	std::cout << "Введите числитель дроби 1: ";
	std::cin >> numerator_1;
	std::cout << "Введите знаменатель дроби 1: ";
	std::cin >> denominator_1;
	std::cout << "Введите числитель дроби 2: ";
	std::cin >> numerator_2;
	std::cout << "Введите знаменатель дроби 2: ";
	std::cin >> denominator_2;

	Fraction f1(numerator_1, denominator_1);
	Fraction f2(numerator_2, denominator_2);
	Fraction Sum = f1 + f2;
	print(f1, f2, Sum, '+');
	Fraction Sub = f1 - f2;
	print(f1, f2, Sub, '-');
	Fraction Mult = f1 * f2;
	print(f1, f2, Mult, '*');
	Fraction Div = f1 / f2;
	print(f1, f2, Div, '/');

	std::cout << "++";
	f1.Print();
	std::cout << " * ";
	f2.Print();
	Fraction expression_1 = ++f1 * f2;
	std::cout << " = ";
	expression_1.Print();
	std::cout << std::endl << "Значение дроби 1 = ";
	f1.Print();
	std::cout << std::endl;

	f1.Print();
	std::cout << "--";
	std::cout << " * ";
	f2.Print();
	Fraction expression_2 = f1-- * f2;
	std::cout << " = ";
	expression_2.Print();
	std::cout << std::endl << "Значение дроби 1 = ";
	f1.Print();
	
	return 0;
}

void print(Fraction& f1, Fraction& f2, Fraction& Result, char operator_type) {
	f1.Print();
	std::cout << " " << operator_type << " ";
	f2.Print();
	std::cout << " = ";
	Result.Print();
	std::cout << std::endl;
}