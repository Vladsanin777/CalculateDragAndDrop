#include <iostream>
#include <iomanip>
#include <cmath>

// Функция для вычисления π с использованием BBP-формулы
double computePiBBP(int numIterations) {
    double pi = 0.0;
    for (int k = 0; k < numIterations; k++) {
        double term = (1.0 / std::pow(16, k)) * (
            4.0 / (8 * k + 1) -
            2.0 / (8 * k + 4) -
            1.0 / (8 * k + 5) -
            1.0 / (8 * k + 6)
        );
        pi += term;

        std::cout << std::setprecision(15) << "Pi (BBP formula): " << pi << std::endl;
    }
    return pi;
}

int main() {
    int numIterations = 10; // Количество итераций
    double pi = computePiBBP(numIterations);

    // Вывод результата с высокой точностью
    std::cout << std::setprecision(15) << "Pi (BBP formula): " << pi << std::endl;

    return 0;
}