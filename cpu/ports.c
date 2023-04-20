#include "cpu/ports.h"

unsigned char port_inb(unsigned short port) {
    unsigned char data;
    asm("in %%dx, %%al" : "=a" (data) : "d" (port));
}

void port_outb(unsigned short port, unsigned char data) {
    asm("out %%al, %%dx" :: "a" (data), "d" (port));
}