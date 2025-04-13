#include <mpfr.h>
#include "Calculate.h"
#include <iostream>

int main() {
    const char *str = new char[100] {"sincosx+2"};
    puts(str);
    Expression::init();
    Expression * expression = Expression::buildExpressionTree(str);
    puts("Expression89");
    puts(expression->print());
    Expression * diffExpression {expression->diff()};
    puts("Expression");
    puts(diffExpression->print());
    mpfr_t result;
    Expression::calculate(diffExpression, result);
    //puts("dgs");
    char * buffer {new char[100]};
    mpfr_sprintf(buffer, "%.77Rf", result);
    puts(buffer);
    //delete expression;
    return 0;
}