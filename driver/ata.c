#include "driver/ata.h"
#include "driver/vga.h"
#include "cpu/ports.h"
#include <string.h>

/*
data_port        = base
error_port       = base + 1
sector_cnt_port  = base + 2
lba_low_port     = base + 3
lba_mid_port     = base + 4
lba_hi_port      = base + 5
dev_port         = base + 6
cmd_port         = base + 7
ctrl_port        = base + 0x206
*/

#define BYTES_PER_SECTOR 512

ata_dev ata_detect(u16int base, u8int master, char* dev_name){
    ata_dev dev;
    dev.base = base;
    dev.dev_name = dev_name;
    dev.master = master;
    dev.ready = 0;

    u8int status;
    printk("ATA name=");
    printk(dev_name);
    printk(" ");
    printk(master ? "MASTER" : "SLAVE");
    printk(": ");

    port_outb(dev.base+6, master ? 0xA0 : 0xB0);
    port_outb(dev.base+0x206, 0);
    port_outb(dev.base+6, 0xA0);

    status = port_inb(dev.base+7);
    if (status == 0xFF) {
        printk("NO DEVICE\n");
        return dev;
    }

    port_outb(dev.base+6, master ? 0xA0 : 0xB0);
    port_outb(dev.base+2, 0);
    port_outb(dev.base+3, 0);
    port_outb(dev.base+4, 0);
    port_outb(dev.base+5, 0);
    port_outb(dev.base+7, 0xEC);

    status = port_inb(dev.base+7);
    if (status == 0xFF) {
        printk("NO DEVICE\n");
        return dev;
    }
    
    while(((status & 0x80) == 0x80) && !(status & 0x40))
        status = port_inb(dev.base+7);
    
    status = port_inb(dev.base+7);
    if(status & 0x01) { 
        printk("ERROR\n");
        return dev;
    }

    /* read 256 word (must) */
    for (u16int i = 0; i < BYTES_PER_SECTOR; i+= 2)
        port_inw(dev.base+7);
        
    dev.ready = 1;
    printk("READY\n");
    return dev;
}

u8int ata_write(ata_dev dev, u32int sector, u8int* data, u16int count){
    if (sector & 0xF0000000) return 0;
    if (count > BYTES_PER_SECTOR) return 0;
    u8int status;

    port_outb(dev.base+6, (dev.master ? 0xE0 : 0xF0) | ((sector & 0x0F000000) >> 24));
    port_outb(dev.base+1, 0);
    port_outb(dev.base+2, 1); /* Always read/write 1 sector */

    port_outb(dev.base+3,  (sector & 0x000000FF));
    port_outb(dev.base+4, ((sector & 0x0000FF00) >> 8));
    port_outb(dev.base+5, ((sector & 0x00FF0000) >> 16));
    port_outb(dev.base+7, 0x30);

    // wait
    status = port_inb(dev.base+7);
    while ((status & 0x80) && !(status & 0x40))
        status = port_inb(dev.base+7);

    for (u16int i = 0; i < count; i+= 2){
        u16int wdata = data[i];
        if (i + 1 < count)
            wdata |= (u16int)data[i+1] << 8;
        port_outw(dev.base, wdata);
    }

    //fill the rest bytes with 0s
    for (u16int i = count + (count  % 2); i < BYTES_PER_SECTOR; i+= 2)
        port_outw(dev.base, 0x0000);

    return 1;
}

u8int ata_read(ata_dev dev, u32int sector, u8int* data, u16int count){
    if (sector & 0xF0000000) return 0;
    u8int status;
    
    port_outb(dev.base+6, (dev.master ? 0xE0 : 0xF0) | ((sector & 0x0F000000) >> 24));
    port_outb(dev.base+1, 0);
    port_outb(dev.base+2, 1); /* Always read/write 1 sector */

    port_outb(dev.base+3,  (sector & 0x000000FF));
    port_outb(dev.base+4, ((sector & 0x0000FF00) >> 8));
    port_outb(dev.base+5, ((sector & 0x00FF0000) >> 16));
    port_outb(dev.base+7, 0x20);

    status = port_inb(dev.base+7);
    if(status & 0x01) { 
        printk("ATA ERROR: cannot read sector\n");
        return 0;
    }

    // wait
    status = port_inb(dev.base+7);
    while ((status & 0x80) && !(status & 0x40))
        status = port_inb(dev.base+7);

    for(u16int i=0;  i< count; i+= 2){
        u16int rdata = port_inw(dev.base);
        data[i] = rdata & 0xFF;
        data[i+1] = (rdata >> 8) & 0xFF;
    }

    for (u16int i = count + (count  % 2) ; i < BYTES_PER_SECTOR; i+= 2)
        port_inw(dev.base);

    return 1;
}

u8int ata_flush(ata_dev dev){
    u8int status;

    port_outb(dev.base+6, (dev.master ? 0xE0 : 0xF0));
    port_outb(dev.base+7, 0xE7); /* flush */

    // wait
    status = port_inb(dev.base+7);
    while ((status & 0x80) && !(status & 0x40))
        status = port_inb(dev.base+7);
    
    if(status & 0x01) { 
        printk("ATA ERROR: cannot flush\n");
        return 0;
    }

    return 1;
}


void init_ata(){
    ata_dev master = ata_detect(0x1F0, 1, "myos");
    ata_dev slave  = ata_detect(0x1F0, 0, "myos");

    // u8int buff[15] = "This is file";
    // ata_write(slave, 5, buff, 14);
    // ata_flush(slave);

    u8int read[15];
    read[14] = '\0';
    printk("READ: ");
    ata_read(slave, 5, read, 14);
    printk(read);

    printk("\n");
}
