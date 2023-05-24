#include <driver/pci.h>
#include <string.h>
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
    pci.name[0] = '\0';

    data = pci_read(bus, device, function, 0x00);
    pci.vendor_id = (data & 0xFFFF);
    pci.device_id = (data >> 16) & 0xFFFF;

    data = pci_read(bus, device, function, 0x04);
    pci.command = (data & 0xFFFF);
    pci.status = (data >> 16) & 0xFFFF;

    data = pci_read(bus, device, function, 0x08);
    pci.class_code = (data >> 24) & 0xFF;
    pci.subclass = (data >> 16) & 0xFF;
    pci.progif = (data >> 8) & 0xFF;
    pci.rev = (data & 0xFF);

    data = pci_read(bus, device, function, 0x0C);
    pci.header_type = (data >> 16) & 0xFF;

    return pci;
}

u8int handle_bar(pci_t* pci){
    bar_t bar;
    u8int head_type = (pci->header_type & 0x3);
    /* Standard Header */
    if (head_type == 0){
        for (u8int bar_i = 0; bar_i < 6; bar_i++){
            u32int data = pci_read(pci->bus, pci->device, pci->function, 0x10 + 4*bar_i);
            bar.type = (data & 0x1) ? mem_map : io_map;
            if (bar.type == io_map){
                bar.addr = (u32int)(data & ~ 0x3);
                // return 1;
            }
        }
    
    /* PCI-to-PCI bridge */
    } else if (head_type == 1) {
    /* Cardbus bridge */
    } else if (head_type == 2) {
    } else {} // unkown
    return 0;
}

void pci_get_name(pci_t* pci){
    char num[2];
    

    switch (pci->class_code)
    {
    case 0x0:
        strcat(pci->name, "Unclassified: ");
        break;

    case 0x1:
        strcat(pci->name, "Storage: ");

    case 0x2:
        strcat(pci->name, "Network: ");
        switch (pci->subclass)
        {
        case 0x0:
            strcat(pci->name, "Ethernet");
            break;
        case 0x1:
            strcat(pci->name, "Token Ring");
            break;
        case 0x2:
            strcat(pci->name, "FDDI");
            break;
        case 0x3:
            strcat(pci->name, "ATM");
            break;
        case 0x4:
            strcat(pci->name, "ISDN");
            break;
        case 0x5:
            strcat(pci->name, "WorldFlp Controller");
            break;
        case 0x6:
            strcat(pci->name, "PICMG 2.14 MultiComputing");
            break;
        case 0x7:
            strcat(pci->name, "Infiniband");
            break;
        case 0x8:
            strcat(pci->name, "Fabric");
            break;
        default:
            strcat(pci->name, "Uknown (0x");
            int_to_hex(pci->subclass, num, 2);
            strcat(pci->name, num);
            strcat(pci->name, ")");
        }
        break;

        break;

    case 0x3:
        strcat(pci->name, "Display: ");
        switch (pci->subclass)
        {
        case 0x0:
            strcat(pci->name, "VGA Controller");
            break;
        case 0x1:
            strcat(pci->name, "XGA Controller");
            break;
        case 0x2:
            strcat(pci->name, "3D Controller");
            break;
        default:
            strcat(pci->name, "Uknown (0x");
            int_to_hex(pci->subclass, num, 2);
            strcat(pci->name, num);
            strcat(pci->name, ")");
        }
        break;

    case 0x4:
        strcat(pci->name, "Multimedia: ");
        break;

    case 0x5:
        strcat(pci->name, "Memory: ");
        break;

    case 0x6:
        strcat(pci->name, "Bridge: ");
        switch (pci->subclass)
        {
        case 0x0:
            strcat(pci->name, "Host");
            break;
        case 0x1:
            strcat(pci->name, "ISA");
            break;
        case 0x2:
            strcat(pci->name, "EISA");
            break;
        case 0x3:
            strcat(pci->name, "MCA");
            break;
        case 0x4:
            strcat(pci->name, "PCI-to-PCI");
            break;
        case 0x5:
            strcat(pci->name, "PCMCIA");
            break;
        case 0x6:
            strcat(pci->name, "NuBus");
            break;
        case 0x7:
            strcat(pci->name, "CardBus");
            break;
        case 0x8:
            strcat(pci->name, "RACEway");
            break;
        case 0x9:
            strcat(pci->name, "PCI-to-PCI");
            break;
        case 0x0A:
            strcat(pci->name, "InfiniBand-to-PCI");
            break;
        default:
            strcat(pci->name, "Uknown (0x");
            int_to_hex(pci->subclass, num, 2);
            strcat(pci->name, num);
            strcat(pci->name, ")");
        }
        break;

    case 0x7:
        strcat(pci->name, "Simple Communication: ");
        break;
    
    case 0x8:
        strcat(pci->name, "Base System Peripheral: ");
        switch (pci->subclass)
        {
        case 0x0:
            strcat(pci->name, "PIC");
            break;
        case 0x1:
            strcat(pci->name, "DMA");
            break;
        case 0x2:
            strcat(pci->name, "Timer");
            break;
        case 0x3:
            strcat(pci->name, "RTC");
            break;
        case 0x4:
            strcat(pci->name, "PCI Hot-Plug");
            break;
        case 0x5:
            strcat(pci->name, "SD Host");
            break;
        case 0x6:
            strcat(pci->name, "IOMMU");
            break;

        default:
            strcat(pci->name, "Uknown (0x");
            char num[2];
            int_to_hex(pci->subclass, num, 2);
            strcat(pci->name, num);
            strcat(pci->name, ")");
            break;
        }
        break;
    
    case 0xC:
        strcat(pci->name, "Serial Bus: ");
        switch (pci->subclass)
        {
        case 0x0:
            strcat(pci->name, "FireWire (IEEE 1394)");
            break;
        case 0x1:
            strcat(pci->name, "ACCESS Bus");
            break;
        case 0x2:
            strcat(pci->name, "SSA");
            break;
        case 0x3:
            strcat(pci->name, "USB");
            break;
        case 0x4:
            strcat(pci->name, "Fibre");
            break;
        case 0x5:
            strcat(pci->name, "SMBus");
            break;
        case 0x6:
            strcat(pci->name, "InfiniBand Controller");
            break;
        case 0x7:
            strcat(pci->name, "IPMI Interface");
            break;
        case 0x8:
            strcat(pci->name, "SERCOS Interface (IEC 61491)");
            break;
        case 0x9:
            strcat(pci->name, "CANbus Controller");
            break;

        default:
            strcat(pci->name, "Uknown (0x");
            int_to_hex(pci->subclass, num, 2);
            strcat(pci->name, num);
            strcat(pci->name, ")");
            break;
        }
        break;


    default:
        break;
    }
    switch (pci->vendor_id)
    {
    case 0x8086:
        strcat(pci->name, " Intel");
        break;
    case 0x1022:
        strcat(pci->name, " AMD ");
        switch (pci->device_id)
        {
        case 0x2000:
            strcat(pci->name, "am79c973");
            break;
        
        default:
            break;
        }
        break;
    
    default:
        break;
    }
}

void pci_probe(){
    for (int bus=0; bus < 256; bus++){
    for (int device=0; device < 32; device++){
    u8int nfunc = pci_has_func(bus, device) ? 8 : 1;

    for (int func=0; func < nfunc; func++){
        pci_t pci = get_pci_dev(bus, device, func);

        if ( pci.vendor_id == 0xFFFF ) break;
        
        printk("PCI ");
        print_int(bus);
        printk(":");
        print_int(device);
        printk(".");
        print_int(func);
        printk(" ");
        print_hex(pci.vendor_id, 4);
        printk(":");
        print_hex(pci.device_id, 4);
        printk(" ");

        pci_get_name(&pci);
        strend(pci.name);
        printk(pci.name);
        if (handle_bar(&pci) == 0) printk(" (NI)");
        
        printk("\n");

    }}}
}

void init_pci(){
    pci_probe();

}
