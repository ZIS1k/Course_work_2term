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
    attrset(COLOR_PAIR(2) | A_BOLD);

    mvprintw(0, 0, "Введите путь к файлу: ");
    clrtoeol();
    refresh();
    getstr(path);

    if ((fPtr = fopen(path, "r")) == NULL) {
        mvprintw(1, 0, "Ошибка: Файл не найден!\n");
        wait_to_continue();
        return;
    }

    if (fgets(buffer, sizeof(buffer), fPtr) == NULL) {
        mvprintw(1, 0, "Ошибка: Пустой файл!\n");
        fclose(fPtr);
        wait_to_continue();
        return;
    }

    token = strtok(buffer, ";");
    if (token == NULL || sscanf(token, "%lf", &lower_limit) != 1) {
        mvprintw(1, 0, "Ошибка: Некорректный нижний предел!\n");
        fclose(fPtr);
        wait_to_continue();
        return;
    }

    token = strtok(NULL, ";");
    if (token == NULL || sscanf(token, "%lf", &upper_limit) != 1) {
        mvprintw(1, 0, "Ошибка: Некорректный верхний предел!\n");
        fclose(fPtr);
        wait_to_continue();
        return;
    }

    token = strtok(NULL, ";");
    if (token == NULL || strlen(token) >= sizeof(expression)) {
        mvprintw(1, 0, "Ошибка: Некорректное выражение!\n");
        fclose(fPtr);
        wait_to_continue();
        return;
    }
    strncpy(expression, token, sizeof(expression)-1);
    expression[sizeof(expression)-1] = '\0';  // Гарантия окончания строки

    fclose(fPtr);

    mvprintw(2, 0, "Загрузка данных прошла успешно!!\n");
    clrtoeol();
    noecho();
    cursor_visibility(FALSE);
    wait_to_continue();
}

void keyboard_input_integral() {
    keypad(stdscr, TRUE);
    cursor_visibility(TRUE);
    echo();
    attrset(COLOR_PAIR(2) | A_BOLD);

    mvprintw(0, 0, "Введите нижный придел интегрирования: ");
    while (scanw("%lf", &lower_limit) != 1) {
        // Очистка буфера до конца строки
        int ch;
        while ((ch = getch()) != '\n' && ch != EOF);
        mvprintw(1, 0, "Ошибка: Введите число.");
        clrtoeol();
        refresh();
        mvprintw(0, 0, "Введите нижный придел интегрирования: ");
        clrtoeol();
        refresh();
    }

    mvprintw(1, 0, "Введите верхний придел интегрирования: ");
    while (scanw("%lf", &upper_limit) != 1) {
        // Очистка буфера до конца строки
        int ch;
        while ((ch = getch()) != '\n' && ch != EOF);
        mvprintw(2, 0, "Ошибка: Введите число.");
        clrtoeol();
        refresh();
        mvprintw(1, 0, "Введите верхний придел интегрирования: ");
        clrtoeol();
        refresh();
    }

    mvprintw(2, 0, "Введите интеграл: ");
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
    attrset(COLOR_PAIR(2) | A_BOLD);

    mvprintw(0, 0, "Введите путь к файлу: ");
    clrtoeol();
    refresh();
    getstr(path);

    fPtr = fopen(path, "w+");
    if (fPtr == NULL) {
        mvprintw(1, 0, "Ошибка: Файл не может быть создан!");
        wait_to_continue();
        return;
    }
    if (strcmp(expression, "Ошибка") != 0){
        fprintf(fPtr, "Lower limit: %.2f\n", lower_limit);
        fprintf(fPtr, "Upper limit: %.2f\n", upper_limit);
        fprintf(fPtr, "Expression: %s\n", expression);
        fprintf(fPtr, "Result: %.4f\n", result);
    } else {
        printw("Критическая ошибка!\n");
        printw("Ошибка при обработке выражения!\n\n");
        printw("Возможные причины:\n");
        printw(" - С момента запуска программы интеграл не был введен.\n");
        printw(" - Во введённом выражении встретилось неожиданная функция.\n");
        printw(" - Во введённом выражении не хватает закрывающихся скобок.\n");
        printw("Вы всё равно можете просмотреть введённое выражение выбрав пункт Вывод интеграла на экран в главном меню\n\n");
    }
    
    fclose(fPtr);

    mvprintw(2, 0, "Результат сохранён в: %s\n", path);
    noecho();
    cursor_visibility(FALSE);
    wait_to_continue();
}