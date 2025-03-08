#include <iostream>
#include "decimal.hpp"

int main() {
    Decimal a("-2");
    Decimal b("8");
    a.printNumber();
    Decimal res = a / b;
    res.printNumber();
    //printf("Sum: %s\n", str); // Output: Sum: 191.34
    //delete[] str;
    return 0;
}