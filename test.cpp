#include "Calculate.h"
#include <cstring>

int main(void) {
    Expression * expr { \
        Expression::buildExpressionTree("4log2", nullptr, false) };
    puts(expr->print());
    puts(expr->calc());
    return 0;
}