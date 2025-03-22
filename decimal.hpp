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
    const static Decimal ZERO, ONE, TWO, FOUR, \
        FIVE, SIX, NUMBER_16, NUMBER_8;
    static Decimal PI, PI_2, PI_2_NEGATIVE;
    static void initializerPI(void) {
        PI = Decimal{"0"};
        char str[11];
        Decimal temp, tempx8;
        //long long templl = 1;
        for (size_t n = 0; n < EPSILON_TRUE; n++) {
            sprintf(str, "%u", n);
            temp = Decimal{str};
            tempx8 = temp * NUMBER_8;
            PI += ONE / NUMBER_16.pow(temp) * ( \
                FOUR / (tempx8 + ONE) - \
                TWO / (tempx8 + FOUR) - \
                ONE / (tempx8 + FIVE) - \
                ONE / (tempx8 + SIX) \
            );
            //PI.printNumber();
            /*
            templl += 2LL;
            sprintf(str, "%lld\0", n % 2 == 0 ? templl : -templl);
            std::cout << "fsf" << std::endl;
            PI += ONE / Decimal{str};
            //PI.printNumber();
            */
        }
        PI_2 = PI * TWO;
        PI_2_NEGATIVE = -PI_2;
        //PI.printNumber();
        return;
    }
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
    void printNumber() const {
        if (_isNegative)
            putchar('-');
        {
            const std::vector<byte>::const_iterator byteIntegerEnd = _integerPart.cbegin() - 1;
            for (
                std::vector<byte>::const_iterator byteInteger = _integerPart.cend() - 1; 
                byteInteger > byteIntegerEnd; byteInteger--
            ) printf("%hhu", *byteInteger);
        }
        putchar('.');
        {
            const std::vector<byte>::const_iterator byteFractionalEnd = _fractionalPart.cend();
            for (
                std::vector<byte>::const_iterator byteFractional = _fractionalPart.cbegin();
                byteFractional < byteFractionalEnd; byteFractional++
            ) printf("%hhu", *byteFractional);
        }
        putchar('\n');
    }
    Decimal& operator+=(const Decimal& other) {
        //printNumber();
        //other.printNumber();
        //std::cout << "addition" << std::endl;
        if (_isNegative != other._isNegative) {
            return *this -= (-other);
        }
        size_t carry = 0;
        const size_t absoluteMax = ~size_t(0);
        // Start Fractional Part
        {
            // Sheach min and max size vectors factional part
            const size_t minFractional = \
                std::min(_fractionalPart.size(), other._fractionalPart.size()) - 1, \
                maxFractional = std::max(_fractionalPart.size(), other._fractionalPart.size());
            //std::cout << "min fractional" << minFractional \
                << "max fractional" << maxFractional << std::endl;
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
            for (; i != minFractional; i--)
                _fractionalPart[i] = maxFractionalPart[i];
            // addition of the remainder
            for (size_t sum; i != absoluteMax; i--) {
                //std::cout << i << std::endl;
                sum = _fractionalPart[i] + other._fractionalPart[i] + carry;
                _fractionalPart[i] = sum % 10;
                carry = sum / 10;
            }
        }
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
                _integerPart[i] = sum % 10;
                carry = sum / 10;
            }
            // sheach the longest operand integer
            const std::vector<byte> &maxIntegerPart = \
                _integerPart.size() > other._integerPart.size() ? _integerPart : other._integerPart;
            // copying remainder operand integer
            for (size_t sum; i < maxInteger; i++) {
                sum = maxIntegerPart[i] + carry;
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
    Decimal operator+(const Decimal& other) const {
        Decimal result {*this};
        return result += other;
    }
    Decimal operator-() const {
        Decimal result = *this;
        result._isNegative = !_isNegative;
        return result;
    }
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
    Decimal operator-(const Decimal& other) const {
        Decimal result{*this};
        return result -= other;
    }
    Decimal& operator*=(const Decimal& other) {

        Decimal otherCopy = other;
        Decimal thisCopy = *this;
        *this = Decimal{"0"};
        while (otherCopy >= ONE)
            *this += thisCopy, otherCopy--;
        while (!otherCopy.isZero()) {
            otherCopy <<= 1;
            thisCopy >>= 1;
            while (otherCopy >= ONE)
                *this += thisCopy, otherCopy--;
        }
        _isNegative = _isNegative != other._isNegative;
        return *this;
    }
    Decimal operator*(const Decimal& other) const {
        Decimal result{*this};
        return result *= other;
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
    Decimal& operator%=(const Decimal& other) {
        if (other.isZero()) throw std::runtime_error("Division by zero");
        while (*this >= other)
            *this -= other;
        return *this;
    }
    Decimal operator%(const Decimal& other) const {
        Decimal result{*this};
        return result %= other;
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
            normalize();
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
        normalize();
        return *this;
    }
    Decimal operator<<(const size_t step) const {
        Decimal result{*this};
        return result <<= step;
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
    Decimal operator>>(const size_t step) const {
        Decimal result{*this};
        return result >>= step;
    }
    Decimal& operator++(int arrgument) {
        *this += ONE;
        return *this;
    }
    Decimal& operator--(int arrgument) {
        *this -= ONE;
        return *this;
    }

    Decimal& operator/=(const Decimal& other) {
        //other.printNumber();
        if (other.isZero()) {
            throw std::invalid_argument("Division by zero");
        }

        Decimal thisCopy{this->absAssignment()};
        *this = Decimal{"0"};
        Decimal otherCopy{other};
        otherCopy.absAssignment();

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
    Decimal operator/(const Decimal& other) const {
        return Decimal{*this} /= other;
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

    Decimal& absAssignment(void) {
        _isNegative = false;
        return *this;
    }
    Decimal abs(void) const {
        return Decimal{*this}.absAssignment();
    }
    bool isEpsilon(void) const {
        for (std::vector<byte>::const_iterator \
            itInteger {this->_integerPart.cbegin()}, \
            itIntegerEnd {this->_integerPart.cend()};
            itInteger != itIntegerEnd; itInteger++
        ) if (*itInteger != 0) return false;
        for (std::vector<byte>::const_iterator \
            itFractional {this->_fractionalPart.cbegin()}, \
            itFractionalEnd {
                this->_fractionalPart.size() < EPSILON_TRUE ? \
                this->_fractionalPart.cend() : \
                itFractional + EPSILON_TRUE
            };
            itFractional != itFractionalEnd; itFractional++
        ) if (*itFractional != 0) return false;
        return true;
    }
    inline bool isNotEpsilon(void) const {
        return !isEpsilon();
    }
    Decimal& logAssignment(void) {
        if (this->_isNegative) 
            throw std::invalid_argument("Invalid number Log");
        std::cout << "start" << std::endl;
        const Decimal term {(*this - ONE) / (*this + ONE)}, \
            termSquared {term * term};
        std::cout << "term ";
        term.printNumber();
        Decimal power{term}, n{ONE}, temp;
        *this = ZERO;
        std::cout << "dfzjk" << std::endl;
        for (size_t i = 0; i < EPSILON_TRUE; i++) {
            std::cout << "dfzjk" << std::endl;
            *this += power / n;
            power *= termSquared;
            n += TWO;
            printNumber();
        }
        std::cout << "logAssignment void" << std::endl;
        return *this *= TWO;
    }
    Decimal log(void) const {
        std::cout<<"log void"<<std::endl;
        return Decimal{*this}.logAssignment();
    }
    Decimal& logAssignment(const Decimal& other) {
        return this->logAssignment() /= other.log();
    }
    Decimal log(const Decimal& other) const {
        return this->log() /= other.log();
    }
    
    Decimal& expAssigment(void) {
        Decimal thisCopy {*this}, term{ONE}, n{ONE};
        *this = ONE;
        while (term.isNotEpsilon()) {
            term *= thisCopy / n;
            *this += term;
            n++;
        }
        return *this;
    }
    Decimal exp(void) const {
        Decimal result{*this};
        return result.expAssigment();
    }
    Decimal& powAssignment(const Decimal& other) {
        if (this->isZero() && ZERO >= other)
            throw std::invalid_argument("Невозможно возвести 0 в неположительную степень.");
        return (other * this->log()).expAssigment();
    }
    Decimal pow(const Decimal& other) const {
        return Decimal{*this}.powAssignment(other);
    }
    Decimal& factorialAssignment(void) {
        Decimal thisCopy{*this};
        *this = ONE;
        for (Decimal i{"1"}; thisCopy >= i; i++)
            *this *= i;
        return *this;
    }
    Decimal factorial(void) {
        return Decimal{*this}.factorialAssignment();
    }
    Decimal& sinAssignment(void) {
        std::cout << "jk;l" << std::endl;
        Decimal thisCopy{*this};
        std::cout << "jk;l" << std::endl;
        *this = ZERO;
        std::cout << "jk;l" << std::endl;
        while (thisCopy > PI_2)
            thisCopy += PI_2_NEGATIVE;
        std::cout << "jk;l" << std::endl;
        while (PI_2_NEGATIVE > thisCopy)
            thisCopy += PI_2;
        size_t n = 0;
        std::cout << "jk;l" << std::endl;
        Decimal temp1, temp2;
        unsigned long long tempull;
        char str[22];
        std::cout << "jk;l" << std::endl;

        do {
            tempull = (unsigned long long)n << 1 + 1;
            sprintf(str, "%llu\0", tempull);
            std::cout << "jkj" << std::endl;
            temp1 = Decimal{str};
            temp2 = thisCopy.pow(temp1) / temp1.factorial();
            *this += n & 1 == 0 ? temp2 : -temp2;
            n++;
        } while (temp2.isNotEpsilon());
        return *this;
    }
    Decimal sin(void) {
        return Decimal{*this}.sinAssignment();
    }
    void normalize(void) {
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
size_t Decimal::EPSILON_FALSE = 400;
const Decimal Decimal::ZERO = Decimal{"0"}, \
    Decimal::ONE = Decimal{"1"}, \
    Decimal::TWO = Decimal{"2"}, \
    Decimal::FOUR = Decimal{"4"}, \
    Decimal::FIVE = Decimal{"5"}, \
    Decimal::SIX = Decimal{"6"}, \
    Decimal::NUMBER_16 = Decimal{"16"}, \
    Decimal::NUMBER_8 = Decimal{"8"};

Decimal Decimal::PI = Decimal::ZERO, \
    Decimal::PI_2 = Decimal::ZERO, \
    Decimal::PI_2_NEGATIVE = Decimal::ZERO;
