#include <mpfr.h>
#include "Calculate.h"
#include <iostream>

int main() {
    mpfr_t num1, num2, result;
    mpfr_init2(num1, 256UL); // Точность в битах
    mpfr_init2(num2, 256UL);
    mpfr_init2(result, 256UL);

    mpfr_set_str(num1, "123456789012345678901234567890.2", 10, MPFR_RNDN);
    mpfr_set_str(num2, "987654321098765432109876543210.1", 10, MPFR_RNDN);

    mpfr_add(result, num1, num2, MPFR_RNDN);

    mpfr_printf("Результат: %.50Rf\n", result);

    mpfr_clear(num1);
    mpfr_clear(num2);
    mpfr_clear(result);

    return 0;
}