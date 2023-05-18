#include "kernel/kernel.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "cpu/timer.h"
#include "driver/vga.h"
#include "driver/keyboard.h"
#include "driver/mouse.h"
#include "driver/ata.h"
#include "driver/pci.h"

void kernel_main(){
    clear_screen();

    printk("Hello World\n");

    printk("Loading GDT...\n");
    init_gdt();

    printk("Loading IDT...\n");
    init_idt();

    printk("Intialising Timer...\n");
    asm volatile("sti");
    init_timer(0);

    printk("Intialising Keyboard...\n");
    init_keyboard();

    printk("Intialising Mouse...\n");
    init_mouse();

    printk("Intialising ATA...\n");
    init_ata();

    printk("Intialising PCI...\n");
    init_pci();

    u8int shell_exit = start_shell();
    printk("Shell Exited with code ");
    char code[3];
    int_to_ascii(shell_exit, code);
    printk(code);
    printk("\n");

    printk("Halt");
    while(1);
}
