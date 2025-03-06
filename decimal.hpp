#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <cmath>
#include <iostream>
#include <cstring>

using byte = unsigned char;

class Decimal {
private:
    std::vector<byte> integerPart;
    std::vector<byte> fractionalPart;
    bool isNegative;
public:
    Decimal(const Decimal& other) = default;
    Decimal() : isNegative(false), integerPart{0} {}
    Decimal(const char * const str) {
        if (*str == '-') isNegative = true;
        else isNegative = false;
        const char * const ptrPoint = strchr(str, '.');
        const size_t len = strlen(str);
        if (!ptrPoint) {
            for (const char *strInteger = str + len - 1; strInteger >= str; strInteger--)
                integerPart.push_back((byte)(*strInteger-'0'));
            fractionalPart = {0};
            return;
        }
        putchar('\n');
        for (const char* strInteger = ptrPoint - 1; strInteger >= str; strInteger--) {
            putchar(*strInteger);
            integerPart.push_back((byte)(*strInteger-'0'));
        }
        putchar('\n');
        const char * const ptrEnd = str + len;
        for (const char* strFractional = ptrPoint + 1; strFractional < ptrEnd; strFractional++) {
            putchar(*strFractional);
            fractionalPart.push_back((byte)(*strFractional-'0'));
        }
        putchar('\n');
    }
    void printNumber() {
        if (isNegative)
            putchar('-');
        {
            const std::vector<byte>::iterator byteIntegerEnd = integerPart.begin() - 1;
            for (
                std::vector<byte>::iterator byteInteger = integerPart.end() - 1; 
                byteInteger > byteIntegerEnd; byteInteger--
            ) printf("%hhu", *byteInteger);
        }
        putchar('.');
        {
            const std::vector<byte>::iterator byteFractionalEnd = fractionalPart.end();
            for (
                std::vector<byte>::iterator byteFractional = fractionalPart.begin();
                byteFractional < byteFractionalEnd; byteFractional++
            ) printf("%hhu", *byteFractional);
        }
        putchar('\n');
    }
    Decimal operator+(const Decimal& other) const {
        if (isNegative != other.isNegative) {
            //return *this - (-other);
        }
        Decimal result;
        result.isNegative = isNegative;
        size_t carry = 0;
        const size_t absoluteMax = ~size_t(0);
        // Start Fractional Part
        {
            // Sheach min and max size vectors factional part
            const size_t minFractional = \
                std::min(fractionalPart.size(), other.fractionalPart.size()) - 1, \
                maxFractional = std::max(fractionalPart.size(), other.fractionalPart.size());
            // Resize result fractional
            result.fractionalPart.resize(maxFractional);
            // define the longest part
            const std::vector<byte> &maxFractionalPart = \
                fractionalPart.size() > other.fractionalPart.size() ? \
                fractionalPart : other.fractionalPart;
            // i it is index digit start from end digit
            size_t i = maxFractional + 1;
            // copying element from the longest part to result \
                to align the fractional part
            for (; i > minFractional; i--)
                result.fractionalPart[i] = maxFractionalPart[i];
            // addition of the remainder
            for (size_t sum; i != absoluteMax; i--) {
                sum = fractionalPart[i] + other.fractionalPart[i] + carry;
                result.fractionalPart[i] = sum % 10;
                carry = sum / 10;
            }
        }
        std::cout << carry << std::endl;
        // End Fractional Part
        // Start Integer Part
        {
            // Sheatch min and max size vectors integer part
            const size_t minInteger = std::min(integerPart.size(), other.integerPart.size()), \
                maxInteger = std::max(integerPart.size(), other.integerPart.size());

            // Resize result integer
            result.integerPart.resize(maxInteger);
            // i it is index digit start from end digit
            size_t i = 0;
            // addition two operand
            for (size_t sum; i < minInteger; i++) {
                sum = integerPart[i] + other.integerPart[i] + carry;
                std::cout << i << (short)integerPart[i] << ' ' << (short)other.integerPart[i] << ' ' << carry << ' ' << sum << std::endl;
                result.integerPart[i] = sum % 10;
                carry = sum / 10;
            }
            // sheach the longest operand integer
            const std::vector<byte> &maxIntegerPart = \
                integerPart.size() > other.integerPart.size() ? integerPart : other.integerPart;
            // copying remainder operand integer
            for (size_t sum; i < maxInteger; i++) {
                sum = maxIntegerPart[i] + carry;
                std::cout << sum << std::endl;
                result.integerPart[i] = sum % 10;
                carry = sum / 10;
            }
            // adding remainder number from main addtion integer
            while (carry != 0) {
                result.integerPart.push_back(carry % 10);
                carry /= 10;
            }
        }
        // End Integer Part

        return result;
    }
};
/*
// Класс который на писал Deepseek это его максимум!!!
class Decimal {
private:
    std::vector<int> integerPart;
    std::vector<int> fractionalPart;
    bool isNegative;
    static const int PRECISION = 20;

    void normalize() {
        // Удаление ведущих нулей
        auto it = integerPart.begin();
        while (it != integerPart.end() && *it == 0) it++;
        integerPart.erase(integerPart.begin(), it);
        if (integerPart.empty()) integerPart.push_back(0);

        // Удаление хвостовых нулей
        while (!fractionalPart.empty() && fractionalPart.back() == 0)
            fractionalPart.pop_back();
    }

    void alignFractions(std::vector<int>& f1, std::vector<int>& f2) const {
        size_t max_len = std::max(f1.size(), f2.size());
        f1.resize(max_len, 0);
        f2.resize(max_len, 0);
    }

public:
    Decimal() : isNegative(false), integerPart{0} {}

    Decimal(const std::string& s) {
        size_t start = 0;
        isNegative = false;

        if (s[start] == '-') {
            isNegative = true;
            start++;
        }

        size_t dot = s.find('.');
        std::string intPart = s.substr(start, (dot == std::string::npos) ? s.size() - start : dot - start);
        std::string fracPart = (dot == std::string::npos) ? "" : s.substr(dot + 1);

        for (char c : intPart) {
            if (!isdigit(c)) throw std::invalid_argument("Invalid decimal format");
            integerPart.push_back(c - '0');
        }

        for (char c : fracPart) {
            if (!isdigit(c)) throw std::invalid_argument("Invalid decimal format");
            fractionalPart.push_back(c - '0');
        }

        normalize();
    }

    Decimal operator+(const Decimal& other) const {
        if (isNegative != other.isNegative) {
            Decimal tmp = other;
            tmp.isNegative = !isNegative;
            return *this - tmp;
        }

        Decimal result;
        result.isNegative = isNegative;

        // Сложение дробных частей
        std::vector<int> frac1 = fractionalPart;
        std::vector<int> frac2 = other.fractionalPart;
        alignFractions(frac1, frac2);

        int carry = 0;
        std::vector<int> resFrac(frac1.size());
        for (int i = frac1.size() - 1; i >= 0; --i) {
            int sum = frac1[i] + frac2[i] + carry;
            resFrac[i] = sum % 10;
            carry = sum / 10;
        }
        result.fractionalPart = resFrac;

        // Сложение целых частей
        size_t maxInt = std::max(integerPart.size(), other.integerPart.size());
        std::vector<int> int1(maxInt, 0);
        std::vector<int> int2(maxInt, 0);

        std::copy(integerPart.rbegin(), integerPart.rend(), int1.rbegin());
        std::copy(other.integerPart.rbegin(), other.integerPart.rend(), int2.rbegin());

        std::vector<int> resInt;
        for (size_t i = 0; i < maxInt; ++i) {
            int sum = int1[i] + int2[i] + carry;
            resInt.push_back(sum % 10);
            carry = sum / 10;
        }
        if (carry > 0) {
            resInt.push_back(carry);
        }

        std::reverse(resInt.begin(), resInt.end());
        result.integerPart = resInt;

        result.normalize();
        return result;
    }

    Decimal operator-(const Decimal& other) const {
        if (*this == other) return Decimal("0");
        if (isNegative != other.isNegative) {
            Decimal tmp = other;
            tmp.isNegative = !tmp.isNegative;
            return *this + tmp;
        }

        bool abs_less = abs() < other.abs();
        Decimal result = abs_less ? (other - *this) : (*this + (-other));

        if (abs_less)
            result.isNegative = !isNegative;

        result.normalize();
        return result;
    }

    Decimal operator*(const Decimal& other) const {
        // Реализация умножения
        std::vector<int> num1 = integerPart;
        num1.insert(num1.end(), fractionalPart.begin(), fractionalPart.end());
        std::vector<int> num2 = other.integerPart;
        num2.insert(num2.end(), other.fractionalPart.begin(), other.fractionalPart.end());

        std::vector<int> result(num1.size() + num2.size(), 0);

        for (int i = num1.size() - 1; i >= 0; i--) {
            int carry = 0;
            for (int j = num2.size() - 1; j >= 0; j--) {
                int product = num1[i] * num2[j] + result[i + j + 1] + carry;
                result[i + j + 1] = product % 10;
                carry = product / 10;
            }
            result[i] += carry;
        }

        Decimal finalResult;
        size_t decimal_pos = fractionalPart.size() + other.fractionalPart.size();
        finalResult.integerPart.assign(result.begin(), result.end() - decimal_pos);
        finalResult.fractionalPart.assign(result.end() - decimal_pos, result.end());
        finalResult.isNegative = isNegative != other.isNegative;
        finalResult.normalize();
        return finalResult;
    }

    bool operator==(const Decimal& other) const {
        return integerPart == other.integerPart && 
               fractionalPart == other.fractionalPart &&
               isNegative == other.isNegative;
    }

    bool operator<(const Decimal& other) const {
        if (isNegative != other.isNegative) return isNegative;
        if (integerPart.size() != other.integerPart.size())
            return integerPart.size() < other.integerPart.size();
        for (size_t i = 0; i < integerPart.size(); i++) {
            if (integerPart[i] != other.integerPart[i])
                return integerPart[i] < other.integerPart[i];
        }
        for (size_t i = 0; i < std::max(fractionalPart.size(), other.fractionalPart.size()); i++) {
            int a = i < fractionalPart.size() ? fractionalPart[i] : 0;
            int b = i < other.fractionalPart.size() ? other.fractionalPart[i] : 0;
            if (a != b) return a < b;
        }
        return false;
    }

    Decimal abs() const {
        Decimal result = *this;
        result.isNegative = false;
        return result;
    }

        // Добавляем унарный оператор минус
    Decimal operator-() const {
        Decimal result(*this);
        result.isNegative = !result.isNegative;
        return result;
    }

    // Добавляем оператор деления (базовая реализация)
    Decimal operator/(const Decimal& other) const {
        if (other == Decimal("0")) {
            throw std::invalid_argument("Division by zero");
        }
        
        Decimal dividend = this->abs();
        Decimal divisor = other.abs();
        Decimal result;
        result.isNegative = isNegative != other.isNegative;

        // Упрощённая реализация деления (только для демонстрации)
        int counter = 0;
        while (dividend >= divisor) {
            dividend = dividend - divisor;
            counter++;
        }
        result.integerPart = {counter};
        
        result.normalize();
        return result;
    }

    // Добавляем недостающие операторы сравнения
    bool operator>(const Decimal& other) const {
        return other < *this;
    }

    bool operator>=(const Decimal& other) const {
        return !(*this < other);
    }

    bool operator<=(const Decimal& other) const {
        return !(other < *this);
    }

    std::string toString() const {
        std::string result;
        if (isNegative) result += '-';
        for (int d : integerPart) result += std::to_string(d);
        if (!fractionalPart.empty()) {
            result += '.';
            for (int d : fractionalPart) result += std::to_string(d);
        }
        return result;
    }

    char* toCharArray() const {
        std::string s = toString();
        char* result = new char[s.size() + 1];
        std::copy(s.begin(), s.end(), result);
        result[s.size()] = '\0';
        return result;
    }

    // Математические функции
    Decimal sqrt() const {
        if (isNegative) throw std::domain_error("sqrt of negative number");
        
        Decimal x(*this);
        Decimal prev;
        Decimal eps("0.0000001");
        
        do {
            prev = x;
            x = (x + *this / x) * Decimal("0.5");
        } while ((prev - x).abs() > eps);
        
        return x;
    }

    Decimal pow(const Decimal& exponent) const {
        // Базовая реализация через умножение
        Decimal result("1");
        for (Decimal i("0"); i < exponent; i = i + Decimal("1")) {
            result = result * *this;
        }
        return result;
    }
};
*/