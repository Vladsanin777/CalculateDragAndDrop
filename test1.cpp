#include <mpfr.h>
#include "Calculate.h"
#include <iostream>

int main() {
    const char *str = new char[100] {"-(sincosx+2)*8"};
    puts(str);
    Expression::init();
    Expression * expression = Expression::buildExpressionTree(str, 0L);
    puts("Expression89");
    const char * const strTemp{expression->print()};
    puts(strTemp);
    delete [] strTemp;
    Expression * diffExpression {expression->integrate()};
    delete expression;
    puts("Expression");
    const char *strRes {diffExpression->print()};
    puts(strRes);
    delete [] strRes;
    diffExpression->calc();
    
    const char * const strRes2 {diffExpression->calc()};
    std::cout << (void*)strRes2 << std::endl;
    std::cout << &strRes2 << std::endl;
    puts(strRes2);
    delete [] strRes2;
    
    puts("ffd");
    delete diffExpression;
    puts("dgs");

    //delete expression;
    return 0;
}