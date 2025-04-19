#ifndef INTEGRAL_H
#define INTEGRAL_H

#define INPUT_VALUE(MESSAGE, FORMAT, VARIABLE, addCONDITION, ERR_MESSAGE) \
    printf(MESSAGE); \
    while (scanw(FORMAT, VARIABLE) != 1 || addCONDITION) { \
        printf(ERR_MESSAGE); \
        while (getchar() != '\n'); \
    } \
    while (getchar() != '\n');

void keyboard_input_integral(void);

#endif