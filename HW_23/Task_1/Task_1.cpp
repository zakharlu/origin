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
};

int main()
{
	Fraction f1(4, 3);
	Fraction f2(11, 6);

	std::cout << "f1" << ((f1 == f2) ? " == " : " not == ") << "f2" << '\n';
	std::cout << "f1" << ((f1 != f2) ? " != " : " not != ") << "f2" << '\n';
	std::cout << "f1" << ((f1 < f2) ? " < " : " not < ") << "f2" << '\n';
	std::cout << "f1" << ((f1 > f2) ? " > " : " not > ") << "f2" << '\n';
	std::cout << "f1" << ((f1 <= f2) ? " <= " : " not <= ") << "f2" << '\n';
	std::cout << "f1" << ((f1 >= f2) ? " >= " : " not >= ") << "f2" << '\n'; 
	return 0;
}