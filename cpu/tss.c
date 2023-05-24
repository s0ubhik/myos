#include <cpu/tss.h>
#include <cpu/gdt.h>
#include <mem.h>

extern uint32_t tss_kernel_stack;
static tss_t tss;

void tss_flush()
{
    asm volatile("mov %0, %%ax" : : "i"(GDT_ENTRY_TSS));
    asm volatile("ltr %ax");;
}

void gdt_setup_tss(u8int num)
{
    memset(&tss, 0, sizeof(tss));
    tss.esp0 = tss_kernel_stack;
    tss.ss0 = GDT_ENTRY_KERNEL_DATA;

    gdt_set_entry(
        num,
        &tss, 
        sizeof(tss), 
        GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_DESCRIPTOR_TSS,
        GDT_FLAG_GRANULARITY_1B
	);
}
