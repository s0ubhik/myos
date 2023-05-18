#include <kernel/shell.h>
#include <driver/keyboard.h>
#include "driver/ata.h"

int start_shell(){
    set_keyboard_hook(generic_keyboard_hook);
    printk("\nShell v0.1\n");
    while (1) {
        printkc("myos> ", 0x0a);
        char* cmd = input();
        printk("\n");

        if (strcmp(cmd, "") == 0) continue;
        if (strcmp(cmd, "clear") == 0) {
            clear_screen();
        } else if (strcmp(cmd, "exit") == 0) {
            return 1;
        } else if (strcmp(cmd, "ata") == 0) {
            init_ata();
        }
    }
}
