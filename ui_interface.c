#include <ncurses.h>
#include <stdbool.h>

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
    "Load from File", keyboard_input_integral // исправить на чтение файла
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
    attron(COLOR_PAIR(1) | A_BOLD); // включаем пресет 1 (для стандартизации)
}

void redraw_menu_selected_item(int y, char* item_name) {
    move(1 + y, 0);
    deleteln();
    insdelln(1);
    attrset(COLOR_PAIR(1) | A_BOLD);
    printw(" -> %s\n", item_name);
}

void redraw_menu_item(int y, char* item_name) {
    move(1 + y, 0);
    deleteln();
    insdelln(1);
    attrset(COLOR_PAIR(2) | A_DIM);
    printw("    %s\n", item_name);
}

void show_main_menu(){
    insdelln(1);
    move(1, 0);
    int selected_point = 0;
    for (int i = 0; i < MAIN_MENU_ITEM_COUNT; i++){
        if (i == selected_point){
            attrset(COLOR_PAIR(1) | A_BOLD);
            printw(" -> %s\n", main_menu_item[i].item_name);
        } else {
            attrset(COLOR_PAIR(2) | A_DIM);
            printw("    %s\n", main_menu_item[i].item_name);
        }
    }

    int key;

    while(key!=ESC){
        key = getch();

        switch (key)
        {
        case KEY_DOWN:
            selected_point = selected_point != (MAIN_MENU_ITEM_COUNT - 1) ? ++selected_point : 0;
            redraw_menu_item(1 + selected_point, main_menu_item[selected_point].item_name);
            redraw_menu_selected_item(1 + selected_point, main_menu_item[selected_point].item_name);
            break;
        case KEY_UP:
            selected_point = selected_point != 0 ? --selected_point : (MAIN_MENU_ITEM_COUNT - 1);
            redraw_menu_selected_item(1 + selected_point, main_menu_item[selected_point].item_name);
            redraw_menu_item(1 + selected_point, main_menu_item[selected_point].item_name);
            break;
        case ENTER:
            clear();
            main_menu_item[selected_point].handler();
            break;
        }
    refresh();
    }
    clear();
}

void show_input_method_menu(){
    insdelln(1);
    move(1, 0);
    int selected_point = 0;
    for (int i = 0; i < INPUT_METHOD_MENU_ITEM_COUNT; i++){
        if (i == selected_point){
            attrset(COLOR_PAIR(1) | A_BOLD);
            printw(" -> %s\n", input_method_menu_item[i].item_name);
        } else {
            attrset(COLOR_PAIR(2) | A_DIM);
            printw("    %s\n", input_method_menu_item[i].item_name);
        }
    }

    int key;

    while(key!=ESC){
        key = getch();

        switch (key)
        {
        case KEY_DOWN:
            selected_point = selected_point != (INPUT_METHOD_MENU_ITEM_COUNT - 1) ? ++selected_point : 0;
            redraw_menu_item(1 + selected_point, input_method_menu_item[selected_point].item_name);
            redraw_menu_selected_item(1 + selected_point, input_method_menu_item[selected_point].item_name);
            break;
        case KEY_UP:
            selected_point = selected_point != 0 ? --selected_point : (INPUT_METHOD_MENU_ITEM_COUNT - 1);
            redraw_menu_selected_item(1 + selected_point, input_method_menu_item[selected_point].item_name);
            redraw_menu_item(1 + selected_point, input_method_menu_item[selected_point].item_name);
            break;
        case ENTER:
            clear();
            input_method_menu_item[selected_point].handler();
            break;
        }
    refresh();
    }
    clear();
}

void wait_to_continue(){
    printw("Goodbye!\nPress any key to continue");
    keypad(stdscr, TRUE);
    cursor_visibility(FALSE);
    getch();
}

void exit_program() {
    wait_to_continue();
    exit(0);
}

void show_integral(){
    printw("╭──────────────────────────────────────────────────────────╮\n");
    printw("│ Your integral: ∫[%.2f, %.2f] %12s dx │\n", lower_limit, upper_limit, expression);
    printw("╰──────────────────────────────────────────────────────────╯\n");
}

void show_integration_result(){
    // Integration();

    printw("╭──────────────────────────────────────────────────────────╮\n");
    printw("│ ∫[%.2f, %.2f] %12s dx = %.4f │\n", lower_limit, upper_limit, expression, result);
    printw("╰──────────────────────────────────────────────────────────╯\n");
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