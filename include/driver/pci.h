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

    u8int header_type;

    u8int interrupt;
    char name[100];
} pci_t;

typedef enum bar_type_e {
    mem_map = 0,
    io_map = 1
} bar_type;

typedef struct BAR 
{
    bar_type type;
    u8int prefectable;
    u32int addr;
} bar_t;

void init_pci();
#endif