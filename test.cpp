#include <iostream>
#include "decimal.hpp"

int main() {
    Decimal a("12.1");
    Decimal b("688.2");
    Decimal c = a + b;
    char* str = c.toCharArray();
    printf("Sum: %s\n", str); // Output: Sum: 191.34
    delete[] str;
    return 0;
}