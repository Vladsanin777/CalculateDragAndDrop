#include <mpfr.h>
#include "Calculate.h"
#include <iostream>

int main() {
    const char *str = new char[100] {"sincosx+2"};
    puts(str);
    Expression::init();
    Expression * expression = Expression::buildExpressionTree(str);
    puts("Expression89");
    //puts(expression->print());
    Expression * diffExpression {expression->diff()};
    delete expression;
    puts("Expression");
    const char *str {diffExpression->print()};
    puts(str);
    delete [] str;
    Expression *result {diffExpression.calc()};
    delete diffExpression;
    str = result->print();
    delete [] str;
    //puts("dgs");

    //delete expression;
    return 0;
}