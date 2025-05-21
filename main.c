#include <locale.h>
#include <ncurses.h>

#include "globals.h"
#include "ui_interface.h"

void main(){
    setlocale(LC_ALL, "ru_RU.UTF-8");
    initscr();

    initialization_ui();

    show_main_menu();

    endwin();
}