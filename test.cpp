#include <iostream>
#include "decimal.hpp"

int main() {
    //std::vector<unsigned char> integerPart = {0}, fractionalPart = {0, 2};
    //integerPart.insert(integerPart.begin(), fractionalPart.rbegin(), \
        fractionalPart.rend());
    //Decimal a("38726743.817391632123");
    //Decimal b("472.26518");
    //Decimal a{"56.02"}, b{"56"};
    //Decimal a {"897.75432"}, b {"544.355"};
    Decimal a{"0.02"}, b{"2"};
    //a.printNumber();
    //Decimal a{"0.463829592123"}, b{"0.47226518"};
    //a <<= 3;
    //a.printNumber();
    //std::cout << " / " << std::endl;
    //b.printNumber();
    //std::cout << " = " << std::endl;
    (a / b).printNumber();
    //Decimal a{"0.463829592123"}, b{"0.47226518"};
    //std::cout << (a >= b) << std::endl;
    // Decimal res = a / b;
    // res.printNumber();
    //printf("Sum: %s\n", str); // Output: Sum: 191.34
    //delete[] str;
    return 0;
}