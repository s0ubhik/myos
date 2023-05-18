#ifndef MOUSE_H
#define MOUSE_H
#include "cpu/ports.h"
#include "cpu/idt.h"
#include "driver/vga.h"
#include "types.h"

u8int mouse_cycle=0;
s8int mouse_byte[3];
s8int mouse_x=0;
s8int mouse_y=0;

#define PACKETS_IN_PIPE 1024
#define DISCARD_POINT 32

#define MOUSE_PORT   0x60
#define MOUSE_STATUS 0x64
#define MOUSE_ABIT   0x02
#define MOUSE_BBIT   0x01
#define MOUSE_WRITE  0xD4
#define MOUSE_F_BIT  0x20
#define MOUSE_V_BIT  0x08


void mouse_write(u8int write) {
	mouse_wait(1);
	port_outb(MOUSE_STATUS, MOUSE_WRITE);
	mouse_wait(1);
	port_outb(MOUSE_PORT, write);
}

u8int mouse_read() {
	mouse_wait(0);
	char t = port_inb(MOUSE_PORT);
	return t;
}

void mouse_wait(u8int a_type) {
	u32int timeout = 100000;
	if (!a_type) {
		while (--timeout) {
			if ((port_inb(MOUSE_STATUS) & MOUSE_BBIT) == 1) {
				return;
			}
		}
		return;
	} else {
		while (--timeout) {
			if (!((port_inb(MOUSE_STATUS) & MOUSE_ABIT))) {
				return;
			}
		}
		return;
	}
}
void mouse_callback(struct regs *r) {
  switch(mouse_cycle)
  {
    case 0:
      mouse_byte[0]=port_inb(0x60);
      mouse_cycle++;
      break;
    case 1:
      mouse_byte[1]=port_inb(0x60);
      mouse_cycle++;
      break;
    case 2:
      mouse_byte[2]=port_inb(0x60);
      mouse_x=mouse_byte[1];
      mouse_y=mouse_byte[2];
      mouse_cycle=0;
      break;
  }
	irq_ack(IRQ12);
}

void init_mouse() {
	u8int status;

	mouse_wait(1);
	port_outb(MOUSE_STATUS, 0xA8);

	mouse_wait(1);
	port_outb(MOUSE_STATUS, 0x20);

	mouse_wait(0);
	status = port_inb(0x60) | 2;

	mouse_wait(1);
	port_outb(MOUSE_STATUS, 0x60);
	
	mouse_wait(1);
	port_outb(MOUSE_PORT, status);
	
	mouse_write(0xF6);
	mouse_read();
	mouse_write(0xF4);
	mouse_read();
	
	register_interrupt_handler(IRQ12, mouse_callback);
}

#endif
