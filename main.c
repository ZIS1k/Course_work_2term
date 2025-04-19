#include <locale.h>
#include "globals.h"
#include "ui_interface.h"

void main(){
    setlocale(LC_ALL, "");
    initscr();

    initialization_ui();

    show_main_menu();

    endwin();
}