#ifndef PORTS_H
#define PORTS_H

unsigned char port_inb(unsigned short port);
void port_outb(unsigned short port, unsigned char data);

unsigned short port_inw(unsigned short port);
void port_outw(unsigned short port, unsigned short data);

unsigned long port_inl(unsigned short port);
void port_outl(unsigned short port, unsigned long data);

#endif