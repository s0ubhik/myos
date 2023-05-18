#ifndef ATA_H
#define ATA_H
#include "types.h"

typedef struct ATA_DEV
{
    u16int base;
    u8int master;
    char* dev_name;
    u8int ready;
} ata_dev;

u8int ata_write(ata_dev dev, u32int sector, u8int* data, u16int count);
u8int ata_read(ata_dev dev, u32int sector, u8int* data, u16int count);
u8int ata_flush(ata_dev dev);

ata_dev ata_detect(u16int base, u8int master, char* dev_name);
void ata_init();

#endif