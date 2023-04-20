#include "cpu/gdt.h"

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;

static void flush_gdt();
static void gdt_set_gate(u8int num, u32int base, u32int limit, u8int access, u8int granularity);

void init_gdt(){
    gdt_ptr.limit = sizeof(gdt_entries);
    gdt_ptr.base  = (u32int) &gdt_entries;

   gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
   gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
   gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
   gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
   gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment

   flush_gdt();
}

static void gdt_set_gate(u8int num, u32int base, u32int limit, u8int access, u8int granularity)
{
    gdt_entries[num].base_low     = (base & 0xFFFF);
    gdt_entries[num].base_middle  = (base >> 16) & 0xFF;
    gdt_entries[num].base_high    = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low    = (limit & 0xFFFF);
    gdt_entries[num].granularity  = (limit >> 16) & 0x0F;
    
    gdt_entries[num].granularity |= granularity & 0xF0;
    gdt_entries[num].access       = access;
}

static void flush_gdt() {
    /* Load the GDT */
    asm volatile ("lgdt %0" : : "m" (gdt_ptr));

    /* Intialise and make the far jump */
    asm("movw $0x10, %ax    \n \
        movw %ax, %ds       \n \
        movw %ax, %es       \n \
        movw %ax, %fs       \n \
        movw %ax, %gs       \n \
        ljmp $0x08, $next   \n \
        next:               \n");
}
