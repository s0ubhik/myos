#include "cpu/gdt.h"
#include <cpu/tss.h>

gdt_t gdt[6];
gdt_ptr_t gdt_ptr;

void gdt_set_entry(u8int num, uint32_t base, uint16_t limit, uint8_t access, uint8_t granularity)
{
    gdt[num].base_low     = (base & 0xFFFF);
    gdt[num].base_middle  = (base >> 16) & 0xFF;
    gdt[num].base_high    = (base >> 24) & 0xFF;

    gdt[num].limit_low    = (limit & 0xFFFF);
    gdt[num].granularity  = granularity;
    gdt[num].access       = access;
}

static void gdt_flush() {
    /* Load the GDT */
    asm volatile ("lgdt %0" : : "m" (gdt_ptr));

    /* Set kernel registers */
    asm("movw $0x10, %ax    \n \
        movw %ax, %ds       \n \
        movw %ax, %es       \n \
        movw %ax, %fs       \n \
        movw %ax, %gs       \n \
        ljmp $0x08, $next   \n \
        next:               \n");
}


void gdt_init()
{
    printk("Initialising GDT...\n");

    gdt_ptr.base = &gdt;
    gdt_ptr.limit = sizeof(gdt) - 1;

    gdt_set_entry(0, 0, 0, 0, 0);

    gdt_set_entry(
        1, 0x0000, 0xFFFF, 
        GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_CODE_READABLE,
        GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K);

    gdt_set_entry(
        2, 0x0000, 0xFFFF,
        GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_DATA_SEGMENT | GDT_ACCESS_DATA_WRITEABLE,
        GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K);
    
    gdt_set_entry(
        3, 0x0000, 0xFFFF,
        GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_CODE_READABLE,
        GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K);

    gdt_set_entry(
        4, 0x0000, 0xFFFF,
        GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_DATA_SEGMENT | GDT_ACCESS_DATA_WRITEABLE,
        GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4K);
    
    gdt_setup_tss(5);
    
    gdt_flush();
    tss_flush();
}