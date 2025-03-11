#include <iostream>
#include "decimal.hpp"

int main() {
    Decimal a("56.02");
    Decimal b("56");
    a.printNumber();
    (a / b).printNumber();
    // Decimal res = a / b;
    // res.printNumber();
    //printf("Sum: %s\n", str); // Output: Sum: 191.34
    //delete[] str;
    return 0;
}