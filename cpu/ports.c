#include "cpu/ports.h"

unsigned char port_inb(unsigned short port) {
    unsigned char data;
    asm("in %%dx, %%al" : "=a" (data) : "d" (port));
    return data;
}

void port_outb(unsigned short port, unsigned char data) {
    asm("out %%al, %%dx" :: "a" (data), "d" (port));
}

unsigned short port_inw(unsigned short port) {
    unsigned short data;
    asm("inw %%dx, %%ax" : "=a" (data) : "d" (port));
    return data;
}

void port_outw(unsigned short port, unsigned short data) {
    asm("out %%ax, %%dx" :: "a" (data), "d" (port));
}

unsigned long port_inl(unsigned short port) {
    unsigned long data;
    asm("inl %%edx, %%eax" : "=a" (data) : "d" (port));
    return data;
}

void port_outl(unsigned short port, unsigned long data) {
    asm("out %%eax, %%edx" :: "a" (data), "d" (port));
}