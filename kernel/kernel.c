#include "kernel/kernel.h"
#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "cpu/timer.h"
#include "driver/vga.h"
#include "driver/keyboard.h"
#include "driver/mouse.h"
#include "driver/ata.h"
#include "driver/pci.h"

void aa(){
    printkc("H OH O H", 0xb);
}


void kernel_main(unsigned long kernel_stack){
    clear_screen();

    printk("Kernel Stack: 0x");
    print_hex(&kernel_stack+1, 7);
    printk("\n");

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
    

    printk("-------------------------------\n");

    unsigned char* m = (unsigned char*) &aa;
    unsigned char* b = (unsigned char*) 0x600000;

    for (int i=0; i < 45; i++){
        print_hex(m[i], 2);
        b[i] = m[i];
        printk(" ");
    }
    printk("\n");
    for (int i=0; i < 45; i++){
        print_hex(b[i], 2);
        printk(" ");
    }
    printk("\n");

void (*func_ptr)(void) = (void (*)(void))0x600000;
func_ptr();

    printk("\n-------------------------------");

    while(1);
    u8int shell_exit = start_shell();
    printk("Shell Exited with code ");
    char code[3];
    int_to_ascii(shell_exit, code);
    printk(code);
    printk("\n");

    printk("Halt");
    while(1);
}
