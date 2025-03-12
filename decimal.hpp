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
    static size_t EPSILON;
public:
    Decimal(const Decimal& other) = default;
    Decimal() : isNegative(false), integerPart{0}, fractionalPart{0} {}
    Decimal(const char * const str) {
        bool (*condition) (const char * const, const char * const);
        if (*str == '-') {
            condition = []( \
                const char * const operand1, \
                const char * const operand2 \
            ) {
                return operand1 > operand2;
            };
            isNegative = true;
        } else {
            condition = []( \
                const char * const operand1, \
                const char * const operand2 \
            ) {
                return operand1 >= operand2;
            };
            isNegative = false;
        }
        const char * const ptrPoint = strchr(str, '.');
        const size_t len = strlen(str);
        if (!ptrPoint) {
            for (const char *strInteger = str + len - 1; condition(strInteger, str); strInteger--)
                integerPart.push_back((byte)(*strInteger-'0'));
            fractionalPart = {0};
            return;
        }
        putchar('\n');
        for (const char* strInteger = ptrPoint - 1; condition(strInteger, str); strInteger--) {
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
    Decimal& operator+(Decimal& other) {
        if (isNegative != other.isNegative) {
            return *this - (-other);
        }
        puts("addition");
        size_t carry = 0;
        const size_t absoluteMax = ~size_t(0);
        // Start Fractional Part
        {
            // Sheach min and max size vectors factional part
            const size_t minFractional = \
                std::min(fractionalPart.size(), other.fractionalPart.size()) - 1, \
                maxFractional = std::max(fractionalPart.size(), other.fractionalPart.size());
            // Resize result fractional
            fractionalPart.resize(maxFractional);
            // define the longest part
            const std::vector<byte> &maxFractionalPart = \
                fractionalPart.size() > other.fractionalPart.size() ? \
                fractionalPart : other.fractionalPart;
            // i it is index digit start from end digit
            size_t i = maxFractional + 1;
            // copying element from the longest part to result \
                to align the fractional part
            for (; i > minFractional; i--)
                fractionalPart[i] = maxFractionalPart[i];
            // addition of the remainder
            for (size_t sum; i != absoluteMax; i--) {
                sum = fractionalPart[i] + other.fractionalPart[i] + carry;
                fractionalPart[i] = sum % 10;
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
            integerPart.resize(maxInteger);
            // i it is index digit start from end digit
            size_t i = 0;
            // addition two operand
            for (size_t sum; i < minInteger; i++) {
                sum = integerPart[i] + other.integerPart[i] + carry;
                std::cout << i << (short)integerPart[i] << ' ' << (short)other.integerPart[i] << ' ' << carry << ' ' << sum << std::endl;
                integerPart[i] = sum % 10;
                carry = sum / 10;
            }
            // sheach the longest operand integer
            const std::vector<byte> &maxIntegerPart = \
                integerPart.size() > other.integerPart.size() ? integerPart : other.integerPart;
            // copying remainder operand integer
            for (size_t sum; i < maxInteger; i++) {
                sum = maxIntegerPart[i] + carry;
                std::cout << sum << std::endl;
                integerPart[i] = sum % 10;
                carry = sum / 10;
            }
            // adding remainder number from main addtion integer
            while (carry != 0) {
                integerPart.push_back(carry % 10);
                carry /= 10;
            }
        }
        // End Integer Part

        return *this;
    }
    /*
    Decimal operator~() const {
        Decimal result {*this};
        size_t lenght{integerPart.size()};
        for (std::vector<byte>::iterator itInteger {result.integerPart.begin()}; \
            lenght--; itInteger++
        )
            *itInteger = 9 - *itInteger;
        lenght = fractionalPart.size();
        for (std::vector<byte>::iterator itFractional {result.fractionalPart.begin()};
            lenght--; itFractional++
        )
            *itFractional = 9 - *itFractional;
        return result;
    }
    */
    Decimal& operator-() {
        isNegative = !isNegative;
        return *this;
    }
    /*
    Decimal operator-(const Decimal &other) const {
        if (isNegative != other.isNegative) {
            return *this + (-other);
        }
        Decimal result;
        bool isThisLess;
        if (isThisLess = *this < other) result.isNegative = true;
        Decimal &operand1 = isThisLess ? *this : other;
        Decimal &operand2 = isThisLess ? other : *this;
        // Start subtraction Fraction Part
        {
            const size_t minFractional = \
                std::min(operand1.fractionalPart.size(), \
                    operand2.fractionalPart.size()) - 1, \
                maxFractional = std::max(operand.fractionalPart.size(), \
                    operand2.fractionalPart.size());
            result.fractionalPart.resize(maxFractional);
            std::vector<byte>::iterator itFractional = operand1.fractionalPart;
        }
    }
    */
    bool operator>(const Decimal& other) const {
        // Если знаки разные, положительное число всегда больше
        if (!isNegative && other.isNegative) {
            return true;
        }
        if (isNegative && !other.isNegative) {
            return false;
        }

        // Сравнение целых частей
        if (integerPart.size() > other.integerPart.size()) {
            return !isNegative; // Если текущее число длиннее, оно больше (если оба положительные)
        }
        if (integerPart.size() < other.integerPart.size()) {
            return isNegative; // Если текущее число короче, оно меньше (если оба положительные)
        }

        // Поразрядное сравнение целых частей
        for (size_t i = integerPart.size(); i > 0; i--) {
            size_t idx = i - 1;
            if (integerPart[idx] > other.integerPart[idx]) {
                return !isNegative;
            }
            if (integerPart[idx] < other.integerPart[idx]) {
                return isNegative;
            }
        }

        // Сравнение дробных частей
        size_t maxFractional = std::max(fractionalPart.size(), other.fractionalPart.size());
        for (size_t i = 0; i < maxFractional; i++) {
            byte thisDigit = i < fractionalPart.size() ? fractionalPart[i] : 0;
            byte otherDigit = i < other.fractionalPart.size() ? other.fractionalPart[i] : 0;

            if (thisDigit > otherDigit) {
                return !isNegative;
            }
            if (thisDigit < otherDigit) {
                return isNegative;
            }
        }

        // Если числа равны
        return false;
    }
    Decimal& operator-(Decimal& other) {
        puts("predvar substraction");
        if (isNegative != other.isNegative) {
            return *this + (-other);
        }
        puts("substraction");


        // Определяем, какое число больше по модулю
        bool thisIsGreater = (*this > other);

        if (!thisIsGreater) {
            isNegative = !isNegative;
        }

        const Decimal& minuend = thisIsGreater ? *this : other;
        const Decimal& subtrahend = thisIsGreater ? other : *this;

        size_t borrow = 0;

        std::cout<<"launch fractional part" << std::endl;
        // Вычитание дробной части
        {
            size_t maxFractional = std::max(minuend.fractionalPart.size(), subtrahend.fractionalPart.size());
            fractionalPart.resize(maxFractional);

            for (size_t i = maxFractional; i > 0; i--) {
                size_t idx = i - 1;
                byte minuendDigit = idx < minuend.fractionalPart.size() ? minuend.fractionalPart[idx] : 0;
                byte subtrahendDigit = idx < subtrahend.fractionalPart.size() ? subtrahend.fractionalPart[idx] : 0;

                if (minuendDigit < subtrahendDigit + borrow) {
                    fractionalPart[idx] = minuendDigit + 10 - subtrahendDigit - borrow;
                    borrow = 1;
                } else {
                    fractionalPart[idx] = minuendDigit - subtrahendDigit - borrow;
                    borrow = 0;
                }
            }
        }
        std::cout<<"launch integer part" << std::endl;

        // Вычитание целой части
        {
            size_t maxInteger = std::max(minuend.integerPart.size(), subtrahend.integerPart.size());
            integerPart.resize(maxInteger);

            for (size_t i = 0; i < maxInteger; i++) {
                byte minuendDigit = i < minuend.integerPart.size() ? minuend.integerPart[i] : 0;
                byte subtrahendDigit = i < subtrahend.integerPart.size() ? subtrahend.integerPart[i] : 0;

                if (minuendDigit < subtrahendDigit + borrow) {
                    integerPart[i] = minuendDigit + 10 - subtrahendDigit - borrow;
                    borrow = 1;
                } else {
                    integerPart[i] = minuendDigit - subtrahendDigit - borrow;
                    borrow = 0;
                }
            }
        }
          
        std::cout<<"finish integer part" << std::endl;
        // Удаление ведущих нулей в целой части
        while (integerPart.size() > 1 && integerPart.back() == 0) {
            integerPart.pop_back();
        }
        std::cout << "opi" << std::endl;
        // Удаление ведущих нулей в дробной части
        while (fractionalPart.size() > 1 && fractionalPart.back() == 0) {
            fractionalPart.pop_back();
        }

        std::cout << "opa" << std::endl;
        return *this;
    }
    Decimal operator*(const Decimal& other) const {
        Decimal result;
        result.isNegative = isNegative != other.isNegative;

        // Сумма дробных цифр результата
        size_t sum_frac = fractionalPart.size() + other.fractionalPart.size();

        // Преобразуем числа в объединенные векторы в обратном порядке
        std::vector<byte> a = getCombinedReversed();
        std::vector<byte> b = other.getCombinedReversed();

        // Умножаем векторы
        std::vector<byte> product_reversed = multiplyVectors(a, b);

        // Переворачием результат для прямого порядка
        std::vector<byte> product(product_reversed.rbegin(), product_reversed.rend());

        // Разделяем на целую и дробную части
        size_t total_digits = product.size();
        size_t integer_digits = (total_digits > sum_frac) ? (total_digits - sum_frac) : 0;

        // Обработка целой части
        std::vector<byte> new_integer;
        if (integer_digits > 0) {
            new_integer.assign(product.begin(), product.begin() + integer_digits);
            std::reverse(new_integer.begin(), new_integer.end());
            // Удаляем ведущие нули
            while (new_integer.size() > 1 && new_integer.back() == 0) {
                new_integer.pop_back();
            }
        } else {
            new_integer.push_back(0);
        }

        // Обработка дробной части
        std::vector<byte> new_fractional;
        if (sum_frac > 0) {
            size_t frac_start = integer_digits;
            if (frac_start < product.size()) {
                new_fractional.assign(product.begin() + frac_start, product.end());
                // Удаляем хвостовые нули
                while (!new_fractional.empty() && new_fractional.back() == 0) {
                    new_fractional.pop_back();
                }
            }
        }

        result.integerPart = new_integer;
        result.fractionalPart = new_fractional;

        // Проверка на ноль
        if (result.integerPart.size() == 1 && result.integerPart[0] == 0 && result.fractionalPart.empty()) {
            result.isNegative = false;
        }

        return result;
    }
    // Вспомогательная функция для получения объединенного вектора (целая + дробная части) в обратном порядке
    std::vector<byte> getCombinedReversed() const {
        std::vector<byte> combined;

        // Целая часть в прямом порядке
        std::vector<byte> integerForward(integerPart.rbegin(), integerPart.rend());
        combined.insert(combined.end(), integerForward.begin(), integerForward.end());

        // Дробная часть как есть
        combined.insert(combined.end(), fractionalPart.begin(), fractionalPart.end());

        // Переворачиваем для обратного порядка при умножении
        std::reverse(combined.begin(), combined.end());

        return combined;
    }

    // Вспомогательная функция умножения векторов в обратном порядке
    std::vector<byte> multiplyVectors(const std::vector<byte>& a, const std::vector<byte>& b) const {
        std::vector<byte> result(a.size() + b.size(), 0);

        for (size_t i = 0; i < a.size(); ++i) {
            byte carry = 0;
            for (size_t j = 0; j < b.size(); ++j) {
                byte product = a[i] * b[j] + result[i + j] + carry;
                result[i + j] = product % 10;
                carry = product / 10;
            }
            if (carry > 0) {
                result[i + b.size()] += carry;
            }
        }

        // Удаляем ведущие нули (в конце вектора)
        while (result.size() > 1 && result.back() == 0) {
            result.pop_back();
        }

        return result;
    }
    bool operator>=(Decimal& other) {
        if (isNegative && !other.isNegative) return false;
        if (!isNegative && other.isNegative) return true;
        bool (*func) (bool) =  isNegative == other.isNegative ? \
            [] (bool isTrue) -> bool {return isTrue;} : \
            [] (bool isTrue) -> bool {return !isTrue;};
        size_t integerThisSize = integerPart.size(), \
            integerOtherSize = other.integerPart.size();
        if (integerThisSize < integerOtherSize) return false;
        if (integerOtherSize > integerOtherSize) return true;
        for (std::vector<byte>::const_iterator it{integerPart.cbegin()}; it != integerPart.cend(); it++)
            std::cout << (short)*it << std::endl;
        for (std::vector<byte>::const_reverse_iterator integerThis \
            = integerPart.crend()-1, integerOther = \
            other.integerPart.crend()-1; integerOtherSize--; \
            integerThis--, integerOther-- \
        ) {
            printNumber();
            other.printNumber();
            std::cout << "rowjk" << (short)*integerThis << (short)*integerOther << std::endl;
            if (*integerThis == *integerOther) continue;
            std::cout << "kkjjj" << std::endl;
            if (*integerThis > *integerOther) return func(true);
            std::cout << "op" << std::endl;
            return func(false);
        }
        size_t fractionalThisSize = fractionalPart.size(), \
            fractionalOtherSize = other.fractionalPart.size();
        size_t minFractionSize = std::min(fractionalThisSize, fractionalOtherSize);
        for (std::vector<byte>::const_iterator fractionalThis \
            = fractionalPart.cbegin(), fractionalOther = \
            other.fractionalPart.cbegin(); minFractionSize--; \
            fractionalThis++, fractionalOther++ \
        ) {
            //std::cout << "io " << (short)*fractionalThis << ' ' << (short)*fractionalOther << std::endl;
            if (*fractionalThis == *fractionalOther) continue;
            if (*fractionalThis > *fractionalOther) return func(true);
            return func(false);
        }
        if (fractionalThisSize >= fractionalOtherSize) return func(true);
        return func(false);
    }
    Decimal operator%(Decimal& other) {
        if (other.isZero()) throw std::runtime_error("Division by zero");
        while (*this >= other)
            *this - other;
        return *this;
    }
    Decimal& operator<<=(size_t step) {
        const size_t fractionalSize = fractionalPart.size();
        if (step >= fractionalSize) {
            // Вырезаем первые три элемента
            std::vector<byte>::iterator start = fractionalPart.begin(), \
                end = start + step;

            // Вставляем вырезанные элементы в начало целевого вектора в обратном порядке
            integerPart.insert(integerPart.begin(), end, start);

            // Удаляем вырезанные элементы из исходного вектора
            fractionalPart.erase(start, end);
            return *this;
        }
        if (fractionalSize > 0 && fractionalPart[0] != 0) {
            step -= fractionalSize;
            integerPart.insert(integerPart.begin(), fractionalPart.end(), \
                fractionalPart.begin());
            fractionalPart.clear();
        }
        integerPart.insert(integerPart.begin(), step, 0);
        return *this;
    }
    Decimal& operator>>=(size_t step) {
        std::cout << "fvfzklzdf" << integerPart.size() << std::endl;
        for (std::vector<byte>::const_iterator it = integerPart.cbegin(); it != integerPart.cend(); it++)
            printf("%hhu\n", *it);
        const size_t integerSize = integerPart.size();
        if (step <= integerSize) {
            std::cout << "fvfzklzdf" << std::endl;
            // Вырезаем первые три элемента
            std::vector<byte>::iterator start = integerPart.begin(), \
                end = start + step;

            // Вставляем вырезанные элементы в начало целевого вектора в обратном порядке
            fractionalPart.insert(fractionalPart.begin(), \
                std::make_reverse_iterator(end), \
                std::make_reverse_iterator(start));

            // Удаляем вырезанные элементы из исходного вектора
            integerPart.erase(start, end);
            return *this;
        }
        if (integerSize > 1) {
            std::cout << "jhfjk" << std::endl;
            step -= integerSize;
            fractionalPart.insert(fractionalPart.begin(), integerPart.rend(), \
                integerPart.rbegin());
            integerPart.clear();
        } else if (integerSize == 1 && integerPart[0] != 0) {
            step--;
            fractionalPart.insert(fractionalPart.begin(), integerPart[0]);
            integerPart[0] = 0;
        }
        fractionalPart.insert(fractionalPart.begin(), step, 0);
        return *this;
    }
    Decimal& operator++(int arrgument) {
        puts("++");
        Decimal one{"1"};
        *this + one;
        return *this;
    }
    Decimal operator/(Decimal& other) const {
        Decimal thisCopy{*this}, result{"0"};
        while (thisCopy >= other) {
            thisCopy - other, \
                result++; std::cout << "pop";
        }
        std::cout << "mod ";thisCopy.printNumber();
        std::cout << "/operator "; result.printNumber();
        Decimal otherCopy{other};
        for (size_t step{1}; !thisCopy.isZero() && step < EPSILON; step++) {
            Decimal temp{"0"};
            otherCopy >>= 1;
            puts("sg");
            thisCopy.printNumber();
            otherCopy.printNumber();
            std::cout << "lkjh" << (thisCopy >= otherCopy) << std::endl;
            thisCopy.normalize(), otherCopy.normalize();
            while (thisCopy >= otherCopy) {
                thisCopy = thisCopy - otherCopy;
                temp++;
            }
            std::cout << "form";
            temp.printNumber();
            temp >>= step;
            std::cout << "after";
            temp.printNumber();
            std::cout << "step " << step << std::endl;
            result = result + temp;
            std::cout << "result division";
            result.printNumber();
        }
        return result;

    }

    bool isZero() const {
        for (byte d : integerPart) {
            if (d != 0) return false;
        }
        for (byte d : fractionalPart) {
            if (d != 0) return false;
        }
        return true;
    }

    Decimal abs() const {
        Decimal result = *this;
        result.isNegative = false;
        return result;
    }
    void normalize() {
        // Удаление ведущих нулей в целой части (кроме последнего, если число 0)
        while (integerPart.size() > 1 && integerPart.back() == 0) {
            integerPart.pop_back();
        }

        // Удаление хвостовых нулей в дробной части
        while (!fractionalPart.empty() && fractionalPart.back() == 0) {
            fractionalPart.pop_back();
        }
    }
};

size_t Decimal::EPSILON = 400;

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