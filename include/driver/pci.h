#ifndef PCI_H
#define PCI_H
#include "types.h"

typedef struct PCI
{
    u8int bus;
    u8int device;
    u8int function;

    u16int device_id;
    u16int vendor_id;

    u16int status;
    u16int command;

    u8int class_code;
    u8int subclass;
    u8int progif;
    u8int rev;

    u8int interrupt;
} pci_t;

void init_pci();
#endif