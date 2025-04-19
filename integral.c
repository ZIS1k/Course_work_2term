#include <ncurses.h>

#include "integral.h"
#include "globals.h"

void inputIntegral() {
    keypad(stdscr, FALSE);
    cursor_visibility(TRUE);
    echo();

    INPUT_VALUE("Введите нижний предел интегрирования:", "%lf", &lower_limit, 0, "Неверный нижний предел интегрирования, попробуйте ещё раз.");
    INPUT_VALUE("Введите верхний предел интегрирования: ", "%lf", &upper_limit, lower_limit > upper_limit, "Неверный верхний предел интегрирования, попробуйте ещё раз.");
    INPUT_VALUE("Введите интеграл:", "%99s", expression, 0, "Неверный интеграл, попробуйте ещё раз.");

    wait_to_continue();
}

void Integration() {
    result = trapezoidal_integral(lower_limit, upper_limit, expression);
}

double trapezoidal_integral(double a, double b, char* expression) {
    int n = calculation_partitions(a, b);
    const double width = (b - a) / n;
    double trapezoidal_integral = 0;

    for (int step = 0; step < n; step++) {
        const double x1 = a + step * width;
        const double x2 = a + (step + 1) * width;

        trapezoidal_integral += 0.5 * (x2 - x1) * (parsed_function(x1, expression) + parsed_function(x2, expression));
    }
    return trapezoidal_integral;
}

int calculation_partitions(double a, double b) {
    int res_n = (int)((b - a) * 100);
    res_n = (res_n < 10) ? 10 : res_n; // минимум разбиений
    res_n = (res_n > 100000) ? 100000 : res_n; //максимум разбиений

    return res_n;
}