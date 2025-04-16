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
    const char *strRes {diffExpression->print()};
    puts(strRes);
    delete [] strRes;
    const char * const strRes2 = diffExpression->calc();
    std::cout << (void*)strRes2 << std::endl;
    std::cout << &strRes2 << std::endl;
    puts(strRes2);
    delete diffExpression;
    delete [] str;
    delete [] strRes2;
    //puts("dgs");

    //delete expression;
    return 0;
}