#include <boost/multiprecision/cpp_dec_float.hpp>
#include <iostream>
#include <iomanip>
#include <string>
//#include "decimal.hpp"

int main() {
    /*
    //Decimal::initializerPI();
    //std::vector<unsigned char> integerPart = {0}, fractionalPart = {0, 2};
    //integerPart.insert(integerPart.begin(), fractionalPart.rbegin(), \
        fractionalPart.rend());
    //Decimal a("38726743.817391632123");
    //Decimal b("472.26518");
    //Decimal a{"56.02"}, b{"56"};
    //Decimal a {"897.75432"}, b {"544.355"};
    //Decimal a{"0.02"}, b{"2.02"};
    Decimal a{"16"}, b{"1"};
    (a.pow(b)).printNumber();
    //Decimal a{"-7600"}, b{"0.02"};
    //Decimal a{"0.02"}, b{"15200"};
    //a.printNumber();
    //Decimal a{"0.463829592123"}, b{"0.47226518"};
    //a <<= 3;
    //a.printNumber();
    //std::cout << " / " << std::endl;
    //b.printNumber();
    //std::cout << " = " << std::endl;
    //a.sin().printNumber();
    //(a += b).printNumber();
    //Decimal a{"0.463829592123"}, b{"0.47226518"};
    //std::cout << (a >= b) << std::endl;
    // Decimal res = a / b;
    // res.printNumber();
    //printf("Sum: %s\n", str); // Output: Sum: 191.34
    //delete[] str;
    */

    using namespace boost::multiprecision;

    // Запрашиваем у пользователя количество знаков после запятой
    int precision;
    std::cout << "Введите количество знаков после запятой: ";
    std::cin >> precision;

    // Устанавливаем точность для вывода
    std::cout << std::fixed << std::setprecision(precision);

    // Запрашиваем два числа
    std::string num1_str, num2_str;
    std::cout << "Введите первое число: ";
    std::cin >> num1_str;
    std::cout << "Введите второе число: ";
    std::cin >> num2_str;

    // Преобразуем строки в числа с произвольной точностью
    cpp_dec_float_100 num1(num1_str);
    cpp_dec_float_100 num2(num2_str);

    // Запрашиваем арифметическое действие
    char operation;
    std::cout << "Введите операцию (+, -, *, /): ";
    std::cin >> operation;

    // Выполняем операцию
    cpp_dec_float_100 result;
    switch (operation) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 == 0) {
                std::cout << "Ошибка: деление на ноль!" << std::endl;
                return 1;
            }
            result = num1 / num2;
            break;
        default:
            std::cout << "Ошибка: неизвестная операция!" << std::endl;
            return 1;
    }

    // Выводим результат
    std::cout << "Результат: " << result << std::endl;

    return 0;
}