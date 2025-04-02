#include <mpfr.h>
#include "Calculate.h"
#include <iostream>

int main() {
    const char *str = new char[100] {"8+9"};
    Expression * expression = Expression::buildExpressionTree(str);
    mpfr_t
    delete expression;
    return 0;
}