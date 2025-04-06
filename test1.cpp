#include <mpfr.h>
#include "Calculate.h"
#include <iostream>

int main() {
    const char *str = new char[100] {"sincos3+2"};
    puts(str);
    Expression::init();
    Expression * expression = Expression::buildExpressionTree(str);
    mpfr_t result;
    Expression::calculate(expression, result);
    //puts("dgs");
    char * buffer {new char[100]};
    mpfr_sprintf(buffer, "%.77Rf", result);
    puts(buffer);
    //delete expression;
    return 0;
}