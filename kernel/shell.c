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
extern int keylock;

int start_shell(){
    set_keyboard_hook(generic_keyboard_hook);
    printk("\nShell v0.1\n");
    

    while (1) {
        printkc("myos> ", 0x0a);

        char* cmd = input();
        printk("\n");

        if (strcmp(cmd, "") == 0) continue;
        
        else if (strcmp(cmd, "whoami") == 0) {
            printk("root\n");
        }
        else if (strcmp(cmd, "clear") == 0) {
            clear_screen();
        } else if (strcmp(cmd, "loop") == 0) {
            set_keyboard_hook(process_keyboard_hook);
            while(keylock == 0){
                if (keylock){
                     printk("A");
                     while(1);
                }
                else printk("B");
            }
            set_keyboard_hook(generic_keyboard_hook);

        } else if (strcmp(cmd, "led") == 0) {
            char str[2];
            str[1] = '\0';

            while(1){
                for (char c='A'; c < 'Z'; c++) {

                    str[0] = c;
                    for (int i=0; i <= 240; i+= 1)
                        printkc(str, i);
                
                }
            }
        
        } else if (strcmp(cmd, "exit") == 0) {
            return 1;
        } else if (strcmp(cmd, "pci") == 0) {
            init_pci();
        }
    }
}
