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
    std::vector<byte> _integerPart;
    std::vector<byte> _fractionalPart;
    bool _isNegative;
public:
    static size_t EPSILON_TRUE;
    static size_t EPSILON_FALSE;
    const static Decimal ONE;
    Decimal(const Decimal& other) = default;
    Decimal() : _isNegative(false), _integerPart{0} {}
    Decimal(const char * const str) {
        bool (*condition) (const char * const, const char * const);
        if (*str == '-') {
            condition = []( \
                const char * const operand1, \
                const char * const operand2 \
            ) {
                return operand1 > operand2;
            };
            _isNegative = true;
        } else {
            condition = []( \
                const char * const operand1, \
                const char * const operand2 \
            ) {
                return operand1 >= operand2;
            };
            _isNegative = false;
        }
        const char * const ptrPoint = strchr(str, '.');
        const size_t len = strlen(str);
        if (!ptrPoint) {
            for (const char *strInteger = str + len - 1; condition(strInteger, str); strInteger--)
                _integerPart.push_back((byte)(*strInteger-'0'));
            //fractionalPart = {0};
            return;
        }
        //putchar('\n');
        for (const char* strInteger = ptrPoint - 1; condition(strInteger, str); strInteger--) {
            //putchar(*strInteger);
            _integerPart.push_back((byte)(*strInteger-'0'));
        }
        //putchar('\n');
        const char * const ptrEnd = str + len;
        for (const char* strFractional = ptrPoint + 1; strFractional < ptrEnd; strFractional++) {
            //putchar(*strFractional);
            _fractionalPart.push_back((byte)(*strFractional-'0'));
        }
        //putchar('\n');
    }
    void printNumber() {
        if (_isNegative)
            putchar('-');
        {
            const std::vector<byte>::iterator byteIntegerEnd = _integerPart.begin() - 1;
            for (
                std::vector<byte>::iterator byteInteger = _integerPart.end() - 1; 
                byteInteger > byteIntegerEnd; byteInteger--
            ) printf("%hhu", *byteInteger);
        }
        putchar('.');
        {
            const std::vector<byte>::iterator byteFractionalEnd = _fractionalPart.end();
            for (
                std::vector<byte>::iterator byteFractional = _fractionalPart.begin();
                byteFractional < byteFractionalEnd; byteFractional++
            ) printf("%hhu", *byteFractional);
        }
        putchar('\n');
    }
    Decimal& operator+=(const Decimal& other) {
        if (_isNegative != other._isNegative) {
            return *this -= (-other);
        }
        //puts("addition");
        size_t carry = 0;
        const size_t absoluteMax = ~size_t(0);
        // Start Fractional Part
        {
            // Sheach min and max size vectors factional part
            const size_t minFractional = \
                std::min(_fractionalPart.size(), other._fractionalPart.size()) - 1, \
                maxFractional = std::max(_fractionalPart.size(), other._fractionalPart.size());
            // Resize result fractional
            _fractionalPart.resize(maxFractional);
            // define the longest part
            const std::vector<byte> &maxFractionalPart = \
                _fractionalPart.size() > other._fractionalPart.size() ? \
                _fractionalPart : other._fractionalPart;
            // i it is index digit start from end digit
            size_t i = maxFractional - 1;
            // copying element from the longest part to result \
                to align the fractional part
            for (; i > minFractional; i--)
                _fractionalPart[i] = maxFractionalPart[i];
            // addition of the remainder
            for (size_t sum; i != absoluteMax; i--) {
                sum = _fractionalPart[i] + other._fractionalPart[i] + carry;
                _fractionalPart[i] = sum % 10;
                carry = sum / 10;
            }
        }
        //std::cout << carry << std::endl;
        // End Fractional Part
        // Start Integer Part
        {
            // Sheatch min and max size vectors integer part
            const size_t minInteger = std::min(_integerPart.size(), other._integerPart.size()), \
                maxInteger = std::max(_integerPart.size(), other._integerPart.size());

            // Resize result integer
            _integerPart.resize(maxInteger);
            // i it is index digit start from end digit
            size_t i = 0;
            // addition two operand
            for (size_t sum; i < minInteger; i++) {
                sum = _integerPart[i] + other._integerPart[i] + carry;
                // std::cout << i << (short)integerPart[i] << ' ' << (short)other.integerPart[i] << ' ' << carry << ' ' << sum << std::endl;
                _integerPart[i] = sum % 10;
                carry = sum / 10;
            }
            // sheach the longest operand integer
            const std::vector<byte> &maxIntegerPart = \
                _integerPart.size() > other._integerPart.size() ? _integerPart : other._integerPart;
            // copying remainder operand integer
            for (size_t sum; i < maxInteger; i++) {
                sum = maxIntegerPart[i] + carry;
                //std::cout << sum << std::endl;
                _integerPart[i] = sum % 10;
                carry = sum / 10;
            }
            // adding remainder number from main addtion integer
            while (carry != 0) {
                _integerPart.push_back(carry % 10);
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
    Decimal operator-() const {
        Decimal result = *this;
        result._isNegative = !_isNegative;
        return result;
    }
    /*
    Decimal operator-=(const Decimal &other) const {
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
        if (!_isNegative && other._isNegative) {
            return true;
        }
        if (_isNegative && !other._isNegative) {
            return false;
        }

        // Сравнение целых частей

        if (_integerPart.size() > other._integerPart.size()) {
            return !_isNegative; // Если текущее число длиннее, оно больше (если оба положительные)
        }
        if (_integerPart.size() < other._integerPart.size()) {
            return _isNegative; // Если текущее число короче, оно меньше (если оба положительные)
        }

        // Поразрядное сравнение целых частей
        for (size_t i = _integerPart.size(); i > 0; i--) {
            size_t idx = i - 1;
            if (_integerPart[idx] > other._integerPart[idx]) {
                return !_isNegative;
            }
            if (_integerPart[idx] < other._integerPart[idx]) {
                return _isNegative;
            }
        }

        // Сравнение дробных частей
        size_t maxFractional = std::max(_fractionalPart.size(), other._fractionalPart.size());
        for (size_t i = 0; i < maxFractional; i++) {
            byte thisDigit = i < _fractionalPart.size() ? _fractionalPart[i] : 0;
            byte otherDigit = i < other._fractionalPart.size() ? other._fractionalPart[i] : 0;

            if (thisDigit > otherDigit) {
                return !_isNegative;
            }
            if (thisDigit < otherDigit) {
                return _isNegative;
            }
        }

        // Если числа равны
        return false;
    }
    Decimal& operator-=(const Decimal& other) {
        //puts("predvar substraction");
        if (_isNegative != other._isNegative) {
            return *this += (-other);
        }
        //puts("substraction");


        // Определяем, какое число больше по модулю
        bool thisIsGreater = (*this >= other);

        if (!thisIsGreater) {
            _isNegative = !_isNegative;
        }

        const Decimal& minuend = thisIsGreater ? *this : other;
        const Decimal& subtrahend = thisIsGreater ? other : *this;

        size_t borrow = 0;

        //std::cout<<"launch fractional part" << std::endl;
        // Вычитание дробной части
        {
            size_t maxFractional = std::max(minuend._fractionalPart.size(), subtrahend._fractionalPart.size());
            _fractionalPart.resize(maxFractional);

            for (size_t i = maxFractional; i > 0; i--) {
                size_t idx = i - 1;
                byte minuendDigit = idx < minuend._fractionalPart.size() ? minuend._fractionalPart[idx] : 0;
                byte subtrahendDigit = idx < subtrahend._fractionalPart.size() ? subtrahend._fractionalPart[idx] : 0;

                if (minuendDigit < subtrahendDigit + borrow) {
                    _fractionalPart[idx] = minuendDigit + 10 - subtrahendDigit - borrow;
                    borrow = 1;
                } else {
                    _fractionalPart[idx] = minuendDigit - subtrahendDigit - borrow;
                    borrow = 0;
                }
            }
        }
        //std::cout<<"launch integer part" << std::endl;

        // Вычитание целой части
        {
            size_t maxInteger = std::max(minuend._integerPart.size(), subtrahend._integerPart.size());
            _integerPart.resize(maxInteger);

            for (size_t i = 0; i < maxInteger; i++) {
                byte minuendDigit = i < minuend._integerPart.size() ? minuend._integerPart[i] : 0;
                byte subtrahendDigit = i < subtrahend._integerPart.size() ? subtrahend._integerPart[i] : 0;

                if (minuendDigit < subtrahendDigit + borrow) {
                    _integerPart[i] = minuendDigit + 10 - subtrahendDigit - borrow;
                    borrow = 1;
                } else {
                    _integerPart[i] = minuendDigit - subtrahendDigit - borrow;
                    borrow = 0;
                }
            }
        }
        normalize();
        return *this;
    }
    Decimal operator*=(const Decimal& other) {

        Decimal otherCopy = other;
        Decimal thisCopy = *this;
        *this = Decimal{"0"};
        while (otherCopy >= ONE) {
            //otherCopy.printNumber();
            *this += thisCopy, otherCopy--;
        }
        //otherCopy.printNumber();
        printNumber();
        while (!otherCopy.isZero()) {
            otherCopy <<= 1;
            thisCopy >>= 1;
            while (otherCopy >= ONE) {
                std::cout << "kl" << std::endl;
                otherCopy.printNumber();
                *this += thisCopy, otherCopy--;
            }
            printNumber();
        }
        _isNegative = _isNegative != other._isNegative;
        return *this;
    }
    bool operator>=(const Decimal& other) const {
        if (_isNegative && !other._isNegative) return false;
        if (!_isNegative && other._isNegative) return true;
        bool (*func) (bool) =  _isNegative == other._isNegative ? \
            [] (bool isTrue) -> bool {return isTrue;} : \
            [] (bool isTrue) -> bool {return !isTrue;};
        size_t integerThisSize = _integerPart.size(), \
            integerOtherSize = other._integerPart.size();
        if (integerThisSize == 1 && _integerPart.at(0) == 0) integerThisSize = 0;
        if (integerOtherSize == 1 && other._integerPart.at(0) == 0) integerOtherSize = 0;
        if (integerThisSize < integerOtherSize) return func(false);
        if (integerThisSize > integerOtherSize) return func(true);
        //for (std::vector<byte>::const_iterator it{integerPart.cbegin()}; it != integerPart.cend(); it++)
            //std::cout << (short)*it << std::endl;
        for (std::vector<byte>::const_reverse_iterator integerThis \
            = _integerPart.crend()-1, integerOther = \
            other._integerPart.crend()-1; integerOtherSize--; \
            integerThis--, integerOther-- \
        ) {
            //printNumber();
            //other.printNumber();
            //std::cout << "rowjk" << (short)*integerThis << (short)*integerOther << std::endl;
            if (*integerThis == *integerOther) continue;
            //std::cout << "kkjjj" << std::endl;
            if (*integerThis > *integerOther) return func(true);
            //std::cout << "op" << std::endl;
            return func(false);
        }
        size_t fractionalThisSize = _fractionalPart.size(), \
            fractionalOtherSize = other._fractionalPart.size();
        size_t minFractionSize = std::min(fractionalThisSize, fractionalOtherSize);
        for (std::vector<byte>::const_iterator fractionalThis \
            = _fractionalPart.cbegin(), fractionalOther = \
            other._fractionalPart.cbegin(); minFractionSize--; \
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
    Decimal& operator%=(Decimal& other) {
        if (other.isZero()) throw std::runtime_error("Division by zero");
        while (*this >= other)
            *this -= other;
        return *this;
    }
    Decimal& operator<<=(size_t step) {
        const size_t fractionalSize = _fractionalPart.size();
        if (step <= fractionalSize) {
            //std::cout << "pop456" << std::endl;
            // Вырезаем первые три элемента
            std::vector<byte>::iterator start = _fractionalPart.begin(), \
                end = start + step;

            // Вставляем вырезанные элементы в начало целевого вектора в обратном порядке
            _integerPart.insert(_integerPart.begin(), \
                std::make_reverse_iterator(end), \
                std::make_reverse_iterator(start)
            );

            // Удаляем вырезанные элементы из исходного вектора
            _fractionalPart.erase(start, end);
            return *this;
        }
        if (fractionalSize > 1) {
            //std::cout << "pop678" << std::endl;
            step -= fractionalSize;
            //printNumber();
            //std::cout << (short)*integerPart.begin() \
                << fractionalPart.rend() - fractionalPart.rbegin() << std::endl;
            //integerPart.resize(integerPart.size() + fractionalPart.size());
            _integerPart.insert(_integerPart.begin(), _fractionalPart.rbegin(), \
                _fractionalPart.rend());
            _fractionalPart.clear();
        } else if (fractionalSize == 1 && _fractionalPart[0] != 0) {
            //std::cout << "pop691" << std::endl;
            step--;
            _integerPart.insert(_integerPart.begin(), _fractionalPart[0]);
            _fractionalPart.clear();
        }
        //std::cout << "jkjk" << std::endl;
        _integerPart.insert(_integerPart.begin(), step, 0);
        return *this;
    }
    Decimal& operator>>=(size_t step) {
        //std::cout << "fvfzklzdf" << integerPart.size() << std::endl;
        //for (std::vector<byte>::const_iterator it = integerPart.cbegin(); it != integerPart.cend(); it++)
            //printf("%hhu\n", *it);
        const size_t integerSize = _integerPart.size();
        if (step <= integerSize) {
            //std::cout << "fvfzklzdf" << std::endl;
            // Вырезаем первые три элемента
            std::vector<byte>::iterator start = _integerPart.begin(), \
                end = start + step;

            // Вставляем вырезанные элементы в начало целевого вектора в обратном порядке
            _fractionalPart.insert(_fractionalPart.begin(), \
                std::make_reverse_iterator(end), \
                std::make_reverse_iterator(start));

            // Удаляем вырезанные элементы из исходного вектора
            _integerPart.erase(start, end);
            return *this;
        }
        if (integerSize > 1) {
            //std::cout << "jhfjk" << std::endl;
            step -= integerSize;
            _fractionalPart.insert(_fractionalPart.begin(), _integerPart.rbegin(), \
                _integerPart.rend());
            _integerPart.clear();
        } else if (integerSize == 1 && _integerPart[0] != 0) {
            step--;
            _fractionalPart.insert(_fractionalPart.begin(), _integerPart[0]);
            _integerPart[0] = 0;
        }
        _fractionalPart.insert(_fractionalPart.begin(), step, 0);
        return *this;
    }
    Decimal& operator++(int arrgument) {
        //puts("++");
        //Decimal one{"1"};
        *this += ONE;
        return *this;
    }
    Decimal& operator--(int arrgument) {
        //puts("++");
        //Decimal one{"1"};
        *this -= ONE;
        return *this;
    }

    Decimal& operator/=(const Decimal& other) {
        if (other.isZero()) {
            throw std::invalid_argument("Division by zero");
        }

        Decimal thisCopy{this->abs()};
        *this = Decimal{"0"};
        Decimal otherCopy{other};
        otherCopy.abs();

        while (thisCopy >= otherCopy) {
            thisCopy -= otherCopy, \
                (*this)--;
        }
        otherCopy >>= 1;
        otherCopy.normalize();
        for (size_t step{1}; !thisCopy.isZero() && step < EPSILON_TRUE; step++) {
            byte temp{0};
            while (thisCopy >= otherCopy) {
                thisCopy -= otherCopy;
                temp++;
            }
            this->_fractionalPart.push_back(temp);
            otherCopy >>= 1;
            if (step % 16 != 0 || this->_fractionalPart.empty()) continue;
            size_t periodLength = findPeriod(this->_fractionalPart);
            //std::cout << periodLength << std::endl;
            if (periodLength > 0) {
                // Вставка периода
                std::vector<byte> period(
                    this->_fractionalPart.end() - periodLength,
                    this->_fractionalPart.end()
                );
                
                while (step + periodLength <= EPSILON_FALSE) {
                    this->_fractionalPart.insert(
                        this->_fractionalPart.end(),
                        period.begin(),
                        period.end()
                    );
                    step += periodLength;
                }
                
                // Добавление оставшихся цифр
                if (step < EPSILON_FALSE) {
                    size_t remaining = EPSILON_FALSE - step;
                    this->_fractionalPart.insert(
                        this->_fractionalPart.end(),
                        period.begin(),
                        period.begin() + remaining
                    );
                }
                break;
            }
        }
        _isNegative = _isNegative != other._isNegative;
        return *this;
    }

    // Вспомогательная функция для поиска периода
    size_t findPeriod(const std::vector<byte>& digits) const {
        if (digits.empty()) return 0;

        const auto begin = digits.cbegin();
        const auto end = digits.cend();
        const size_t n = digits.size();

        // Максимальная длина периода — половина вектора
        for (size_t k = n >> 4; k <= n >> 1; ++k) {
            bool isPeriod = true;
            // Проверяем, совпадают ли последние k элементов с предыдущими k
            for (size_t i = 0; i < k; ++i) {
                if (*(end - k - k + i) != *(end - k + i)) {
                    isPeriod = false;
                    break;
                }
            }
            if (isPeriod) return k;
        }

        return 0;
    }

    bool isZero() const {
        for (byte d : _integerPart) {
            if (d != 0) return false;
        }
        for (byte d : _fractionalPart) {
            if (d != 0) return false;
        }
        return true;
    }

    Decimal& abs() {
        _isNegative = false;
        return *this;
    }
    void normalize() {
        // Удаление ведущих нулей в целой части (кроме последнего, если число 0)
        while (_integerPart.size() > 1 && _integerPart.back() == 0) {
            _integerPart.pop_back();
        }

        // Удаление хвостовых нулей в дробной части
        while (!_fractionalPart.empty() && _fractionalPart.back() == 0) {
            _fractionalPart.pop_back();
        }
    }
};

size_t Decimal::EPSILON_TRUE = 400;
size_t Decimal::EPSILON_FALSE = 100000;
const Decimal Decimal::ONE = Decimal{"1"};