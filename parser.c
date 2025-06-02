#include <string.h>
#include <math.h>
#include <ctype.h>
#include <setjmp.h>
#include <stdlib.h>
#include "parser.h"

jmp_buf parse_error;

double parser(char** expr, double x) {
    if (!expr || !*expr || **expr == '\0') {
        strncpy(*expr, "error", sizeof(expr) - 1);
        (*expr)[sizeof(expr) - 1] = '\0';
        return 0.0;
    }
    if (setjmp(parse_error) == 0) {
        return parse_additive(expr, x);
    } else {
        strncpy(*expr, "error", sizeof(expr) - 1);
        (*expr)[sizeof(expr) - 1] = '\0';
        return 0.0;
    }
}

void skipping_spaces(char** expr) {
    while (**expr && isspace((unsigned char)**expr)) (*expr)++;
}

double parse_additive(char** expr, double x) {
    double value = parse_multiplicative(expr, x);
    skipping_spaces(expr);
    while (**expr == '+' || **expr == '-') {
        char op = **expr;
        (*expr)++;
        skipping_spaces(expr);
        double rhs = parse_multiplicative(expr, x);
        if (op == '+') value += rhs;
        else value -= rhs;
        skipping_spaces(expr);
    }
    return value;
}

double parse_multiplicative(char** expr, double x) {
    double value = parse_parentheses(expr, x);
    skipping_spaces(expr);
    while (**expr == '*' || **expr == '/') {
        char op = **expr;
        (*expr)++;
        skipping_spaces(expr);
        double rhs = parse_parentheses(expr, x);
        if (op == '*') value *= rhs;
        else if (rhs != 0.0) value /= rhs;
        else longjmp(parse_error, 1);
        skipping_spaces(expr);
    }
    return value;
}

double parse_parentheses(char** expr, double x) {
    skipping_spaces(expr);
    if (**expr == '(') {
        (*expr)++;
        skipping_spaces(expr);
        double value = parse_additive(expr, x);
        skipping_spaces(expr);
        if (**expr == ')') {
            (*expr)++;
            skipping_spaces(expr);
            return value;
        }
        longjmp(parse_error, 1);
    }

    if (**expr == 'x') {
        (*expr)++;
        skipping_spaces(expr);
        return x;
    }

    if (isalpha((unsigned char)**expr)) {
        return parse_function(expr, x);
    }

    return parsing_number(expr);
}

double parsing_number(char** expr) {
    char* end;
    double value = strtod(*expr, &end);
    if (end == *expr) {
        longjmp(parse_error, 1);
    }
    *expr = end;
    skipping_spaces(expr);
    return value;
}


double parse_function(char** expr, double x) {
    skipping_spaces(expr);
    if (strncmp(*expr, "sin", 3) == 0) {
        (*expr) += 3;
        skipping_spaces(expr);
        double arg = parse_additive(expr, x);
        skipping_spaces(expr);
        return sin(arg);
    }
    
    if (strncmp(*expr, "cos", 3) == 0) {
        (*expr) += 3;
        skipping_spaces(expr);
        double arg = parse_additive(expr, x);
        skipping_spaces(expr);
        return cos(arg);
    }
    
    if (strncmp(*expr, "tan", 3) == 0) {
        (*expr) += 3;
        skipping_spaces(expr);
        double arg = parse_additive(expr, x);
        skipping_spaces(expr);
        return tan(arg);
    }

    if (strncmp(*expr, "ctan", 4) == 0) {
        (*expr) += 4;
        skipping_spaces(expr);
        double arg = parse_additive(expr, x);
        skipping_spaces(expr);
        return ( 1 / tan(arg) );
    }

    if (strncmp(*expr, "log", 3) == 0) {
        (*expr) += 3;
        skipping_spaces(expr);
        double arg = parse_additive(expr, x);
        skipping_spaces(expr);
        return log(arg);
    }

    longjmp(parse_error, 1);
    return 0.0;
        
}