#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "integral.h"
#include "parser.h"
#include "globals.h"
#include "ui_interface.h"

#define MAXLINE 256

void clear_input_buffer() {
    int ch;
    while ((ch = getch()) != '\n' && ch != EOF);
}

void file_input_integral() {
    char path[MAXLINE];
    FILE* fPtr;
    char buffer[MAXLINE];
    char* token;

    keypad(stdscr, TRUE);
    cursor_visibility(TRUE);
    echo();
    attrset(COLOR_PAIR(3) | A_BOLD);

    mvprintw(0, 0, "Enter path to file: ");
    clrtoeol();
    refresh();
    getstr(path);

    if ((fPtr = fopen(path, "r")) == NULL) {
        mvprintw(1, 0, "Error: File not found!\n");
        wait_to_continue();
        return;
    }

    if (fgets(buffer, sizeof(buffer), fPtr) == NULL) {
        mvprintw(1, 0, "Error: Empty file!\n");
        fclose(fPtr);
        wait_to_continue();
        return;
    }

    token = strtok(buffer, ";");
    if (token == NULL || sscanf(token, "%lf", &lower_limit) != 1) {
        mvprintw(1, 0, "Error: Invalid lower limit!\n");
        fclose(fPtr);
        wait_to_continue();
        return;
    }

    token = strtok(NULL, ";");
    if (token == NULL || sscanf(token, "%lf", &upper_limit) != 1) {
        mvprintw(1, 0, "Error: Invalid upper limit!\n");
        fclose(fPtr);
        wait_to_continue();
        return;
    }

    token = strtok(NULL, ";");
    if (token == NULL || strlen(token) >= sizeof(expression)) {
        mvprintw(1, 0, "Error: Invalid expression!\n");
        fclose(fPtr);
        wait_to_continue();
        return;
    }
    strncpy(expression, token, sizeof(expression)-1);
    expression[sizeof(expression)-1] = '\0';  // Гарантия окончания строки

    fclose(fPtr);

    mvprintw(2, 0, "Data loaded successfully!\n");
    clrtoeol();
    noecho();
    cursor_visibility(FALSE);
    wait_to_continue();
}

void keyboard_input_integral() {
    keypad(stdscr, TRUE);
    cursor_visibility(TRUE);
    echo();
    attrset(COLOR_PAIR(3) | A_BOLD);

    mvprintw(0, 0, "Enter a lower integration limit: ");
    while (scanw("%lf", &lower_limit) != 1) {
        // Очистка буфера до конца строки
        int ch;
        while ((ch = getch()) != '\n' && ch != EOF);
        mvprintw(1, 0, "Error: Invalid input. Enter a number.");
        clrtoeol();
        refresh();
        mvprintw(0, 0, "Enter a lower integration limit: ");
        clrtoeol();
        refresh();
    }

    mvprintw(1, 0, "Enter the upper limit of integration: ");
    while (scanw("%lf", &upper_limit) != 1) {
        // Очистка буфера до конца строки
        int ch;
        while ((ch = getch()) != '\n' && ch != EOF);
        mvprintw(2, 0, "Error: Invalid input. Enter a number.");
        clrtoeol();
        refresh();
        mvprintw(1, 0, "Enter the upper limit of integration: ");
        clrtoeol();
        refresh();
    }

    mvprintw(2, 0, "Enter the integral: ");
    clrtoeol();
    refresh();
    getstr(expression);

    move(3, 0);
    clrtobot();

    noecho();
    cursor_visibility(FALSE);
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

        char* expr_ptr1 = expression;
        char* expr_ptr2 = expression;
        double y1 = parser(&expr_ptr1, x1);
        double y2 = parser(&expr_ptr2, x2);

        trapezoidal_integral += 0.5 * (x2 - x1) * (y1 + y2);
    }
    return trapezoidal_integral;
}

int calculation_partitions(double a, double b) {
    int res_n = (int)((b - a) * 100);
    res_n = (res_n < 10) ? 10 : res_n; // минимум разбиений
    res_n = (res_n > 100000) ? 100000 : res_n; //максимум разбиений

    return res_n;
}

void save_result_to_file() {
    char path[MAXLINE];
    FILE* fPtr;

    keypad(stdscr, TRUE);
    cursor_visibility(TRUE);
    echo();
    attrset(COLOR_PAIR(3) | A_BOLD);

    mvprintw(0, 0, "Enter path to save file: ");
    clrtoeol();
    refresh();
    getstr(path);

    fPtr = fopen(path, "w+");
    if (fPtr == NULL) {
        mvprintw(1, 0, "Error: Cannot open file for writing!");
        wait_to_continue();
        return;
    }
    if (strcmp(expression, "error") != 0){
        fprintf(fPtr, "Lower limit: %.2f\n", lower_limit);
        fprintf(fPtr, "Upper limit: %.2f\n", upper_limit);
        fprintf(fPtr, "Expression: %s\n", expression);
        fprintf(fPtr, "Result: %.4f\n", result);
    } else {
        printw("ERROR!\n");
        printw("The result was not written to the file, because there were problems with the expression!\n\n");
        printw("Possible causes:\n");
        printw(" - The integral was not set by the user from the beginning of the program\n");
        printw(" - The entered expression lacks closing brackets\n");
        printw(" - The entered expression contains an incorrect functions names\n\n");
    }
    
    fclose(fPtr);

    mvprintw(2, 0, "Result saved to: %s", path);
    noecho();
    cursor_visibility(FALSE);
    wait_to_continue();
}