#ifndef UI_INTERFACE_H
#define UI_INTERFACE_H

typedef void (*event_handler_fn)();

typedef struct {
    char* item_name;
    event_handler_fn handler;
} menu_item_t;

#define MAIN_MENU_ITEM_COUNT 4
#define INPUT_METHOD_MENU_ITEM_COUNT 2
#define ESC 27
#define ENTER 13

void cursor_visibility(bool);

void initialization_ui();

void redraw_menu_selected_item(int, char*);

void redraw_menu_item(int, char*);

void show_main_menu();

void show_input_method_menu();

void wait_to_continue();

void exit_program();

void show_integral();

void show_integration_result();

#endif