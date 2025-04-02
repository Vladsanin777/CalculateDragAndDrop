#include <mpfr.h>
#include "Calculate.h"
#include <iostream>

int main() {
    const char *str = new char[100] {"0.2mod0.1"};
    Expression * expression = Expression::buildExpressionTree(str);
    mpfr_t result;
    Expression::calculate(expression, result);
    puts("dgs");
    char * buffer {new char[100]};
    mpfr_sprintf(buffer, "%Rf", result);
    puts(buffer);
    //delete expression;
    return 0;
}