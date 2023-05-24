#include "cpu/timer.h"
#include "cpu/idt.h"

static void timer_callback(registers_t regs) {
}

void timer_init(u32int freq) {
	printk("Initializing Timer...\n");
	asm volatile("sti");

	register_interrupt_handler(IRQ0, timer_callback);

	port_outb(0x43, 0x36);

	u8int low;
	u8int high;
	if (freq == 0) {
		/* Use the lowest possible frequency*/
		low = 0xFF;
		high = 0xFF;
	} else {
		/* Use a certain frequence */
		u32int div = 1193180 / freq;
		low = (u8int)(div & 0xFF);
		high = (u8int)((div >> 8 )& 0xFF);
	}

	port_outb(0x40, low);
	port_outb(0x40, high);
}