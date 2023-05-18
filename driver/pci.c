#include <driver/pci.h>
#include "libc/string.h"
#include "driver/vga.h"
#include "cpu/ports.h"

#define PORT_PCI_DATA 0xCFC
#define PORT_PCI_CMD 0xCF8

u32int pci_mkid(u8int bus, u8int device, u8int function, u8int reg_offset){
    u32int id = (1 << 31)
    | ((bus & 0xFF) << 16)
    | ((device & 0x1F) << 11)
    | ((function & 0x07) << 8)
    | (reg_offset);
    return id;
}

u32int pci_read(u8int bus, u8int device, u8int function, u8int reg_offset){
    u32int id = pci_mkid(bus, device, function, reg_offset);
    port_outl(PORT_PCI_CMD, id);
    u32int data = port_inl(PORT_PCI_DATA);
    return data;
}

u32int pci_has_func(u8int bus, u8int device){
    return (pci_read(bus, device, 0, 0xC) & (1 << 31));
}

pci_t get_pci_dev(u8int bus, u8int device, u8int function){
    pci_t pci;
    u32int data;
    pci.bus = bus;
    pci.device = device;
    pci.function = function;

    data = pci_read(bus, device, function, 0x00);
    pci.device_id = (data & 0xFFFF);
    pci.vendor_id = (data >> 16) & 0xFFFF;

    data = pci_read(bus, device, function, 0x04);
    pci.status = (data & 0xFFFF);
    pci.command = (data >> 16) & 0xFFFF;

    data = pci_read(bus, device, function, 0x08);
    pci.class_code = (data >> 24) & 0xFF;
    pci.subclass = (data >> 16) & 0xFF;
    pci.progif = (data >> 8) & 0xFF;
    pci.rev = (data & 0xFF);

    data = pci_read(bus, device, function, 0x0C);
    pci.interrupt = (data & 0xFF);

    return pci;
}

void pci_probe(){
    for (int bus=0; bus < 256; bus++){
        for (int device=0; device < 32; device++){

            /* check if unihost or multihost */           
            u8int nfunc = pci_has_func(bus, device) ? 8 : 1;

            for (int func=0; func < nfunc; func++){
                pci_t pci = get_pci_dev(bus, device, func);
                char s[33];

                if ( pci.vendor_id == 0x0000 ||  pci.vendor_id == 0xFFFF) break;
                
                printk("PCI ");
                print_int(bus);
                printk(":");
                print_int(device);
                printk(".");
                print_int(func);
                printk(" DEVICE ID: ");
                print_hex(pci.device_id);
                printk(" VENDOR ID: ");
                print_hex(pci.vendor_id);
                printk("\n");
            }
        }
    }
}

void init_pci(){
    pci_probe();

}