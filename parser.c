#include <string.h>
#include <math.h>
#include <ctype.h>
#include <setjmp.h>

#include "parser.h"

jmp_buf parse_error;

double parser(char** expr, double x) {
    if (expr == NULL || *expr == NULL || **expr == '\0') {
        *expr = "error";
        return 0.0;
    }
    if (setjmp(parse_error) == 0) {
        return parsing_expression(expr, x);
    } else {
        *expr = "error";
        return 0.0;
    }
}

void skipping_spaces(char** expr) {
    while (isspace((unsigned char)**expr)) {
        (*expr)++;
    }
}

double parsing_expression(char** expr, double x) {
    skipping_spaces(expr);

    if (**expr == '(') {
        double result = parse_parentheses(expr, x);
        skipping_spaces(expr);
        return result;
    }

    if (**expr == 'x') {
        (*expr)++;
        return x;
    }

    if (isdigit(**expr) || **expr == '.'){
        return parsing_number(expr);
    }

    if (isalpha(**expr)){
        return parse_trigonometric(expr, x);
    }

    return parse_additive(expr, x);
}

double parse_parentheses(char** expr, double x) {
    (*expr)++; // Пропуск '('
    skipping_spaces(expr);
    double result = parsing_expression(expr, x);
    skipping_spaces(expr);
    if (**expr != ')') {
        longjmp(parse_error, 1);
    }
    (*expr)++;
    return result;
}

double parsing_number(char** expr) {
    double generated_number = 0;

    while (**expr != '\0' && **expr != '.' && **expr != ',' && isdigit(**expr)) {
        generated_number = generated_number * 10 + (**expr - '0');
        (*expr)++;
    }

    if (**expr == '.' || **expr == ',') {
        (*expr)++;
        double fraction = 1;
        while (**expr != '\0' && isdigit(**expr)) {
            fraction /= 10;
            generated_number += ((**expr) - '0') * fraction;
            (*expr)++;
        }
    }
    return generated_number;
}

double parse_additive(char** expr, double x) {
    skipping_spaces(expr);
    double result = parse_multiplicative(expr, x);
    skipping_spaces(expr);
    

    while(**expr == '+' || **expr == '-') {
        char operation = **expr;
        (*expr)++;
        double term = parse_multiplicative(expr, x);
        if (operation == '+'){
            result += term;
        } else {
            result -= term;
        }
        skipping_spaces(expr);
    }
    return result;
}

double parse_multiplicative(char** expr, double x){
    double result = parsing_expression(expr, x);
    skipping_spaces(expr);
    

    while(**expr == '*' || **expr == '/') {
        char operation = **expr;
        (*expr)++;
        double factor = parsing_expression(expr, x);
        if (operation == '*'){
            result *= factor;
        } else {
            result /= factor;
        }
        skipping_spaces(expr);
    }
    return result;
}

double parse_trigonometric(char** expr, double x) {
    skipping_spaces(expr);
    if (strncmp(*expr, "sin", 3) == 0) {
        (*expr) += 3;
        skipping_spaces(expr);
        double arg = parsing_expression(expr, x);
        skipping_spaces(expr);
        return sin(arg);
    }
    else if (strncmp(*expr, "cos", 3) == 0) {
        (*expr) += 3;
        skipping_spaces(expr);
        double arg = parsing_expression(expr, x);
        skipping_spaces(expr);
        return cos(arg);
    }
    else if (strncmp(*expr, "tan", 3) == 0) {
        (*expr) += 3;
        skipping_spaces(expr);
        double arg = parsing_expression(expr, x);
        skipping_spaces(expr);
        return tan(arg);
    }
    else if (strncmp(*expr, "ctan", 4) == 0) {
        (*expr) += 4;
        skipping_spaces(expr);
        double arg = parsing_expression(expr, x);
        skipping_spaces(expr);
        return ( 1 / tan(arg) );
    }
    else {
        longjmp(parse_error, 1);
    }
}