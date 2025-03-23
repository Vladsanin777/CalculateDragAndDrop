#include <gmp.h>
#include <iostream>

int main() {
    mpf_t num1, num2, result;
    mpf_init2(num1, 256UL); // Точность в битах
    mpf_init2(num2, 256UL);
    mpf_init2(result, 256UL);

    mpf_set_str(num1, "123456789012345678901234567890.1234567890123456789000000000000000000000000000000002", 10);
    mpf_set_str(num2, "987654321098765432109876543210.98765432109876543210", 10);

    mpf_mul(result, num1, num2);

    gmp_printf("Результат: %.50Ff\n", result);

    mpf_clear(num1);
    mpf_clear(num2);
    mpf_clear(result);

    return 0;
}