#include <ncurses.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "integral.h"
#include "globals.h"
#include "ui_interface.h"

menu_item_t main_menu_item[MAIN_MENU_ITEM_COUNT] = {
    {"Ввод интеграла", show_input_method_menu},
    {"Вывод интеграла на экран", show_integral},
    {"Вывести результат интегрирования", show_integration_result},
    {"Сохранить результат в файл", save_result_to_file},
    {"Выход", exit_program}
};

menu_item_t input_method_menu_item[MAIN_MENU_ITEM_COUNT] = {
    "Ручной ввод", keyboard_input_integral,
    "Загрузить из файла", file_input_integral,
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

    init_color(16, 1000, 1000, 1000); // создание самого яркого белого
    init_pair(1, COLOR_RED, 16); // 1 цвет в палитре - красные символы на белом фоне
    init_pair(2, COLOR_BLUE, 16); // 2 цвет в палитре - синие символы на белом фоне


    attron(COLOR_PAIR(1) | A_BOLD); // включаем пресет 1 (для стандартизации)
    bkgd(COLOR_PAIR(1)); // светлый фон
    clear();
}

void show_input_method_menu() {
    int key;
    int selected_point = 0;
    int need_redraw = 1;

    do {
        if (need_redraw) {
            clear();
            // Отрисовка подменю
            attrset(COLOR_PAIR(1) | A_DIM);
            printw("╔═════════ Меню выбора ═════════╗\n");
            for (int i = 0; i < INPUT_METHOD_MENU_ITEM_COUNT; i++) {
                if (i == selected_point) {
                    attrset(COLOR_PAIR(1) | A_BOLD);
                    printw("  ➤ %-30s\n", input_method_menu_item[i].item_name);
                } else {
                    attrset(COLOR_PAIR(2) | A_DIM);
                    printw("    %-30s\n", input_method_menu_item[i].item_name);
                }
            }
            attrset(COLOR_PAIR(1) | A_DIM);
            printw("╚═════════ Меню выбора ═════════╝\n");
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
            attrset(COLOR_PAIR(1) | A_DIM);
            printw("╔═════════ Самое главное меню ═════════╗\n");
            for (int i = 0; i < MAIN_MENU_ITEM_COUNT; i++) {
                if (i == selected_point) {
                    attrset(COLOR_PAIR(1) | A_BOLD);
                    printw("  ➤ %-30s\n", main_menu_item[i].item_name);
                } else {
                    attrset(COLOR_PAIR(2) | A_DIM);
                    printw("    %-30s\n", main_menu_item[i].item_name);
                }
            }
            attrset(COLOR_PAIR(1) | A_DIM);
            printw("╚═════════ Самое главное меню ═════════╝\n");
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
                need_redraw = 1;
                break;
            case ESC:
                exit_program();
                return;
        }
    } while (key != ESC);
}

void wait_to_continue(){
    attrset(COLOR_PAIR(1) | A_BOLD);
    printw("Нажмите любую клавишу для продолжения...");
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
    attrset(COLOR_PAIR(2) | A_BOLD);
    printw(" %.2f\n", upper_limit);
    printw("  ⎰ %s dx \n", expression);
    printw(" %.2f\n", lower_limit);
    wait_to_continue();
}

void show_integration_result(){
    Integration();
    attrset(COLOR_PAIR(2) | A_BOLD);
    if (strcmp(expression, "error") == 0) {
        printw("Критическая ошибка!\n");
        printw("Ошибка при обработке выражения!\n\n");
        printw("Возможные причины:\n");
        printw(" - С момента запуска программы интеграл не был введен.\n");
        printw(" - Во введённом выражении встретилось неожиданная функция.\n");
        printw(" - Во введённом выражении не хватает закрывающихся скобок.\n");
        printw("Вы всё равно можете просмотреть введённое выражение выбрав пункт Вывод интеграла на экран в главном меню\n\n");
    } else {
        printw(" %.2f\n", upper_limit);
        printw("  ⎰ %s dx = %lf\n", expression, result);
        printw(" %.2f\n", lower_limit);
    }
    
    wait_to_continue();
}