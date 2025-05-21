#ifndef PARSER_H
#define PARSER_H

double parser(char** expr, double x);
void skipping_spaces(char** expr);

double parse_additive(char** expr, double x);
double parse_multiplicative(char** expr, double x);
double parse_parentheses(char** expr, double x);
double parsing_number(char** expr);
double parse_function(char** expr, double x);


#endif