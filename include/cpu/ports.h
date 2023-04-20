#ifndef PORTS_H
#define PORTS_H

unsigned char port_inb(unsigned short port);
void port_outb(unsigned short port, unsigned char data);

/* ports */

// Screen
#define PORT_SCREEN_CTRL 0x3d4
#define PORT_SCREEN_DATA 0x3d5

#endif