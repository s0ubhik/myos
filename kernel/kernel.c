#include "kernel/kernel.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "cpu/timer.h"
#include "driver/vga.h"
#include "driver/keyboard.h"
#include "driver/mouse.h"
#include "driver/ata.h"
#include "driver/pci.h"
#include <functions.h>

extern void start_user_mode();

void kernel_main(unsigned long kernel_stack){
    clear_screen();

    printk("Kernel Stack: 0x");
    print_hex(&kernel_stack+1, 7);
    printk("\n");

    gdt_init();
    idt_init();
    timer_init(0);

    printk("Initialising PS2 Keyboard...\n");
    init_keyboard();

    printk("Initialising PS2 Mouse...\n");
    init_mouse();

    printk("Initialising ATA...\n");
    init_ata();

    printk("Initialising PCI...\n");
    init_pci();

    u8int shell_exit = start_shell();
    // printk("Shell Exited with code ");
    // char code[3];
    // int_to_ascii(shell_exit, code);
    // printk(code);
    // printk("\n");

    // printk("Halt");
    // start_user_mode();

    while(1);
}
