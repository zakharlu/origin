#include <iostream>
#include <string>
#include <algorithm>

class big_integer {
private:
    std::string digits;

public:
    big_integer() : digits("0") {} 
    big_integer(const std::string& num) : digits(num) {
        if (digits.empty()) digits = "0";
    }

    big_integer(const big_integer& other) : digits(other.digits) {}

    big_integer& operator=(const big_integer& other) {
        if (this != &other) {
            digits = other.digits;
        }
        return *this;
    }

    big_integer(big_integer&& other) noexcept : digits(std::move(other.digits)) {
        other.digits = "0";
    }

    big_integer& operator=(big_integer&& other) noexcept {
        if (this != &other) {
            digits = std::move(other.digits);
            other.digits = "0";
        }
        return *this;
    }


    big_integer operator+(const big_integer& other) const {
        std::string result;
        int carry = 0; // перенос в следующий разряд (0 или 1)
        int i = digits.size() - 1;
        int j = other.digits.size() - 1;

        while (i >= 0 || j >= 0 || carry > 0) { // пока есть цифры в первом или во втором числе или есть перенос
            int x = (i >= 0) ? (digits[i--] - '0') : 0; // Берем цифру из первого числа (если есть) или 0
            int y = (j >= 0) ? (other.digits[j--] - '0') : 0; // Берем цифру из второго числа (если есть) или 0. digits[i--] - '0' 0преобразуеv символ цифры в число
            int sum = x + y + carry; // сумма текущих цифр и переноса
            carry = sum / 10; //вычисляем перенос в следующий разряд (1 если sum ≥ 10)
            result.push_back((sum % 10) + '0'); // Добавляем символ в конец результата.  (sum % 10) + '0' - преобразуем цифру обратно в символ. 
        }

        std::reverse(result.begin(), result.end()); // Так как мы добавляли цифры от младших к старшим, переворачиваем строку
        return big_integer(result);
    }

    // Оператор умножения на число (int)
    big_integer operator*(int multiplier) const {
        if (multiplier == 0) return big_integer("0");

        big_integer result("0");
        big_integer temp(*this);  // Здесь требуется копирование!

        while (multiplier > 0) {
            if (multiplier % 2 == 1) {
                result = result + temp;
            }
            temp = temp + temp;
            multiplier /= 2;
        }

        return result;
    }


    friend std::ostream& operator<<(std::ostream& os, const big_integer& num) {
        os << num.digits;
        return os;
    }
};


int main() {
    auto number1 = big_integer("114575");
    auto number2 = big_integer("78524");
    auto result = number1 + number2;
    std::cout << result << std::endl; // Д.б.193099

    auto mult = number1 * 3;
    std::cout << mult << std::endl; // Д.б. 343725

    // Проверка перемещения
    big_integer num3 = std::move(number1);
    std::cout << num3 << std::endl; // Д.б. 114575
    std::cout << number1 << std::endl; // Д.б. 0

    return 0;
}