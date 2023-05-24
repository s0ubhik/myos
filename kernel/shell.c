#include <kernel/shell.h>
#include <driver/keyboard.h>
#include <string.h>
#include "driver/pci.h"

testok1(){
    printkc("HOOO\n", 0xc);
}
testok2(){
    printkc("HEHEHE\n", 0xd);
}
extern int test();

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
        } else if (strcmp(cmd, "pci") == 0) {
            init_pci();
        }
    }
}
