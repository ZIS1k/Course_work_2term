#include <ncurses.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "integral.h"
#include "globals.h"
#include "ui_interface.h"

menu_item_t main_menu_item[MAIN_MENU_ITEM_COUNT] = {
    {"Integral Input", show_input_method_menu},
    {"Integral Output", show_integral},
    {"Integration Result", show_integration_result},
    {"Exit", exit_program}
};

menu_item_t input_method_menu_item[MAIN_MENU_ITEM_COUNT] = {
    "Keyboard Input", keyboard_input_integral,
    "Load from File", file_input_integral,
};

void cursor_visibility(bool ch) {
    echo(); // Сначала включаем echo (это важно!)
    curs_set(ch); // полностью/отобразить в виде подчёркивания скрыть курсор
    noecho(); // Затем снова выключаем при необходимости
}

void initialization_ui(){
    start_color(); // включаем палитру
    keypad(stdscr, TRUE); // обработка командных клавиш
    cursor_visibility(false);
    init_pair(1, COLOR_RED, COLOR_WHITE); // 1 цвет в палитре - красные символы на белом фоне
    init_pair(2, COLOR_RED, COLOR_BLACK); // 2 цвет в палитре - красные символы на чёрном фоне
    init_pair(3, COLOR_BLUE, COLOR_WHITE); // 3 цвет в палитре - синие символы на белом фоне
    attron(COLOR_PAIR(1) | A_BOLD); // включаем пресет 1 (для стандартизации)
}

void show_input_method_menu() {
    int key;
    int selected_point = 0;
    int need_redraw = 1;

    do {
        if (need_redraw) {
            clear();
            // Отрисовка подменю
            for (int i = 0; i < INPUT_METHOD_MENU_ITEM_COUNT; i++) {
                if (i == selected_point) {
                    attrset(COLOR_PAIR(1) | A_BOLD);
                    printw(" -> %s\n", input_method_menu_item[i].item_name);
                } else {
                    attrset(COLOR_PAIR(2) | A_DIM);
                    printw("    %s\n", input_method_menu_item[i].item_name);
                }
            }
            refresh();
            need_redraw = 0;
        }

        key = getch();

        switch (key) {
            case KEY_DOWN:
            case KEY_UP:
                selected_point ^= 1;
                need_redraw = 1;
                break;
            case ENTER:
                clear();
                input_method_menu_item[selected_point].handler();
                return;
            case ESC:
                return;
        }
    } while (key != ESC);
}

void show_main_menu() {
    int key;
    int selected_point = 0;
    int need_redraw = 1;

    do {
        if (need_redraw) {
            clear();
            // Отрисовка меню
            for (int i = 0; i < MAIN_MENU_ITEM_COUNT; i++) {
                if (i == selected_point) {
                    attrset(COLOR_PAIR(1) | A_BOLD);
                    printw(" -> %s\n", main_menu_item[i].item_name);
                } else {
                    attrset(COLOR_PAIR(2) | A_DIM);
                    printw("    %s\n", main_menu_item[i].item_name);
                }
            }
            refresh();
            need_redraw = 0;
        }

        key = getch();

        switch (key) {
            case KEY_DOWN:
                selected_point = (selected_point + 1) % MAIN_MENU_ITEM_COUNT;
                need_redraw = 1;
                break;
            case KEY_UP:
                selected_point = (selected_point - 1 + MAIN_MENU_ITEM_COUNT) % MAIN_MENU_ITEM_COUNT;
                need_redraw = 1;
                break;
            case ENTER:
                clear();
                main_menu_item[selected_point].handler();
                need_redraw = 1;  // Требуем перерисовку после возврата
                break;
            case ESC:
                exit_program();
                return;
        }
    } while (key != ESC);
}

void wait_to_continue(){
    printw("Press any key to continue");
    keypad(stdscr, TRUE);
    cursor_visibility(FALSE);
    refresh();
    getch();
    clear();
    refresh();
}

void exit_program() {
    wait_to_continue();
    exit(0);
}

void show_integral(){
    printw("#==========================================================#\n");
    printw("| Your integral: ,/'[%.2f, %.2f] %12s dx |\n", lower_limit, upper_limit, expression);
    printw("#==========================================================#\n");
    wait_to_continue();
}

void show_integration_result(){
    Integration();
    if (strcmp(expression, "error") == 0) {
        printw("ERROR!\n");
        printw("There was an error decoding your expression!\n\n");
        printw("Possible causes:\n");
        printw(" - The integral was not set by the user from the beginning of the program\n");
        printw(" - The entered expression lacks closing brackets\n");
        printw(" - The entered expression contains an incorrect functions names\n\n");
    } else {
        printw("#==========================================================#\n");
        printw("| ,/'[%.2f, %.2f] %12s dx = %.4f |\n", lower_limit, upper_limit, expression, result);
        printw("#==========================================================#\n");
    }
    
    wait_to_continue();
}

/*
int clear();
int getch();
scanw();
attron(COLOR_PAIR(1) | A_BOLD); // включаем пресет 1
attrset(COLOR_PAIR(2) | A_DIM); // невыбранные пункты
int insdelln(int n)// для положительного n вставляет n пустых строк для отрицательного удаляет n строк.
int move(int y, int x)//перемещение курсора
int deleteln() //Удаление строки
getyx(stdscr, int y, int x)// Получение координат курсора
*/