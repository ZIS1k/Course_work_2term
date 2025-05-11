#ifndef PARSER_H
#define PARSER_H

double parser(char**, double );

void skipping_spaces(char**);

double parsing_expression(char**, double);

double parse_parentheses(char**, double);

double parsing_number(char**);

double parse_additive(char**, double x);

double parse_multiplicative(char**, double x);

double parse_trigonometric(char**, double x);

#endif