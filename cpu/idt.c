#include "cpu/idt.h"
#include "cpu/gdt.h"
#include "cpu/ports.h"
#include <mem.h>
#include <string.h>
#include "driver/vga.h"

struct idt_entry_struct idt[256];
struct idt_ptr_struct idt_ptr;
isr_t interrupt_handlers[256];

extern void syscall_handle();


void test(){
    printkc("SYSCALL", 0xc);
}


enum syscallTable
{
    SYS_OUT = 1,
    SYS_IN,
    SYS_MALLOC,
    SYS_FREE
};

void check_syscall( registers_t registers)
{  
    printkc("SYSCALL\n", 0xd);
    switch (registers.eax)
    {
        case SYS_OUT:
            // sysOut(&registers);
            break;

        case SYS_IN:
        {
            u32int value = 11;
            // uint32_t value = sysIn(&registers);
            asm volatile("mov %0, %%eax" : : "m"(value));
            break;
        }

        case SYS_MALLOC:
        {
            void* ptr = 32;
            // void* ptr = sysMalloc(&registers); 
            asm volatile("mov %0, %%eax" : : "m"(ptr));
            break;
        }

        case SYS_FREE:
            // sysFree(&registers);
            break;

        default:
            // badSyscall(__func__);
            break;
    }
}

void noHandle(){
    printk("Giii");
}

void idt_init() {
    printk("Initialising IDT...\n");

    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base = (u32int) &idt;

    memset(&idt, 0, sizeof(idt));

    idt_set_gate(0, (u32int)isr0, 0x08, 0x8E);
    idt_set_gate(1, (u32int)isr1, 0x08, 0x8E);
    idt_set_gate(2, (u32int)isr2, 0x08, 0x8E);
    idt_set_gate(3, (u32int)isr3, 0x08, 0x8E);
    idt_set_gate(4, (u32int)isr4, 0x08, 0x8E);
    idt_set_gate(5, (u32int)isr5, 0x08, 0x8E);
    idt_set_gate(6, (u32int)isr6, 0x08, 0x8E);
    idt_set_gate(7, (u32int)isr7, 0x08, 0x8E);
    idt_set_gate(8, (u32int)isr8, 0x08, 0x8E);
    idt_set_gate(9, (u32int)isr9, 0x08, 0x8E);
    idt_set_gate(10, (u32int)isr10, 0x08, 0x8E);
    idt_set_gate(11, (u32int)isr11, 0x08, 0x8E);
    idt_set_gate(12, (u32int)isr12, 0x08, 0x8E);
    idt_set_gate(13, (u32int)isr13, 0x08, 0x8E);
    idt_set_gate(14, (u32int)isr14, 0x08, 0x8E);
    idt_set_gate(15, (u32int)isr15, 0x08, 0x8E);
    idt_set_gate(16, (u32int)isr16, 0x08, 0x8E);
    idt_set_gate(17, (u32int)isr17, 0x08, 0x8E);
    idt_set_gate(18, (u32int)isr18, 0x08, 0x8E);
    idt_set_gate(19, (u32int)isr19, 0x08, 0x8E);
    idt_set_gate(20, (u32int)isr20, 0x08, 0x8E);
    idt_set_gate(21, (u32int)isr21, 0x08, 0x8E);
    idt_set_gate(22, (u32int)isr22, 0x08, 0x8E);
    idt_set_gate(23, (u32int)isr23, 0x08, 0x8E);
    idt_set_gate(24, (u32int)isr24, 0x08, 0x8E);
    idt_set_gate(25, (u32int)isr25, 0x08, 0x8E);
    idt_set_gate(26, (u32int)isr26, 0x08, 0x8E);
    idt_set_gate(27, (u32int)isr27, 0x08, 0x8E);
    idt_set_gate(28, (u32int)isr28, 0x08, 0x8E);
    idt_set_gate(29, (u32int)isr29, 0x08, 0x8E);
    idt_set_gate(30, (u32int)isr30, 0x08, 0x8E);
    idt_set_gate(31, (u32int)isr30, 0x08, 0x8E);

    port_outb(0x20, 0x11);
    port_outb(0xA0, 0x11);
    port_outb(0x21, 0x20);
    port_outb(0xA1, 0x28);
    port_outb(0x21, 0x04);
    port_outb(0xA1, 0x02);
    port_outb(0x21, 0x01);
    port_outb(0xA1, 0x01);
    port_outb(0x21, 0x0);
    port_outb(0xA1, 0x0);
    
    // kernel inturrupts
    idt_set_gate(32, (u32int)irq0, 0x08, 0x8E);
    idt_set_gate(33, (u32int)irq1, 0x08, 0x8E);
    idt_set_gate(34, (u32int)irq2, 0x08, 0x8E);
    idt_set_gate(35, (u32int)irq3, 0x08, 0x8E);
    idt_set_gate(36, (u32int)irq4, 0x08, 0x8E);
    idt_set_gate(37, (u32int)irq5, 0x08, 0x8E);
    idt_set_gate(38, (u32int)irq6, 0x08, 0x8E);
    idt_set_gate(39, (u32int)irq7, 0x08, 0x8E);
    idt_set_gate(40, (u32int)irq8, 0x08, 0x8E);
    idt_set_gate(41, (u32int)irq9, 0x08, 0x8E);
    idt_set_gate(42, (u32int)irq10, 0x08, 0x8E);
    idt_set_gate(43, (u32int)irq11, 0x08, 0x8E);
    idt_set_gate(44, (u32int)irq12, 0x08, 0x8E);
    idt_set_gate(45, (u32int)irq13, 0x08, 0x8E);
    idt_set_gate(46, (u32int)irq14, 0x08, 0x8E);
    idt_set_gate(47, (u32int)irq15, 0x08, 0x8E);
    idt_set_gate(47, (u32int)irq15, 0x08, 0x8E );

    // syscall
    idt_set_gate(0x80, syscall_handle, 0x08, 0xEF);

    flush_idt();
}

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};




void flush_idt() {
    /* Load the IDT */
    asm volatile ("lidt %0" : : "m" (idt_ptr));
}

void idt_set_gate(u8int num, u32int base, u16int sel, u8int flags)
{
   idt[num].base_lo = base & 0xFFFF;
   idt[num].base_hi = (base >> 16) & 0xFFFF;

   idt[num].sel     = sel;
   idt[num].always0 = 0;
   idt[num].flags   = flags;
}

void isr_handler(registers_t reg){
    printk("Received Interrupt: ");
    char s[3];
    int_to_ascii(reg.int_no, s);
    printk(s);
    printk("\n");
    printk(exception_messages[reg.int_no]);
    printk("\n");
    while(1);
}

void register_interrupt_handler(u8int n, isr_t handler) {
    interrupt_handlers[n] = handler;
}


void irq_handler(registers_t r) {
    // if (r.int_no == 80) {
        // char a[5];
        // int_to_ascii(r.int_no, a);
        // printk("INT ");
        // printk(a);
        // printk("\n");
        // while(1);     
    // }

    irq_ack(r.int_no);
    if (interrupt_handlers[r.int_no] != 0) {
        isr_t handler = interrupt_handlers[r.int_no];
        if (r.int_no > 32) r.int_no -= 32; /* actual IRQ inttupt no */
        handler(r);
    }
}

void irq_ack(u8int int_no){
    if (int_no >= 40) port_outb(0xA0, 0x20); /* for slave */
    port_outb(0x20, 0x20); /* for master */
}

