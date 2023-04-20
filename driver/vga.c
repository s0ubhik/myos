#include "driver/vga.h"
#include "cpu/ports.h"

u8int color = 0x0F;

void clear_screen() {
    unsigned char* vd_mem = (unsigned char*) VGA_ADDRESS;
    for (int i=0; i < VGA_MAX_OFFSET; i=i+2){
        vd_mem[i] = ' ';
        vd_mem[i+1] = 0x00;
    }
    set_cursor_offset(0); /* at the right-most corner */
}

u16int vga_scroll() {
    unsigned char* vd_mem = (unsigned char*) VGA_ADDRESS;
    u16int scroll = MAX_COL*2;
    
    for (int i=0; i < VGA_MAX_OFFSET; i++)
        vd_mem[i] = vd_mem[i+scroll];

    return cal_offset(MAX_ROW - 1,0);
}

void printk(char* str) {
    unsigned char* vd_mem = (unsigned char*) VGA_ADDRESS;
    u16int offset = get_cursor_offset();
    // u8int color = (int) colsor;
    for (int i=0; str[i] != '\0'; i++) {

        if (str[i] == '\n') {
            /* move cursor to right-most column of next the row */
            offset = cal_offset(cal_row(offset)+1 , 0);
        }
        else if (str[i] == '\b'){
            /* move cursor 1 row back */
            vd_mem[offset-2] = ' ';
            vd_mem[offset-1] = color;
            offset = cal_offset(cal_row(offset), cal_col(offset)-1);
        }
        else {
            vd_mem[offset] = str[i];
            vd_mem[++offset] = color; /* color attribute */
            offset++;
        }

        /* scroll if cursor reached end of the screen */
        if (offset >= VGA_MAX_OFFSET) offset = vga_scroll();

    }

    set_cursor_offset(offset);
}

void printkc(char *str, u8int _color) {
    u8int oldcol = color;
    color = _color;
    printk(str);
    color = oldcol;
}

u16int get_cursor_offset() {
    u16int offset;
    
    port_outb(PORT_SCREEN_CTRL, 14);
    offset = port_inb(PORT_SCREEN_DATA) << 8;

    port_outb(PORT_SCREEN_CTRL, 15);
    offset |=  port_inb(PORT_SCREEN_DATA);
    
    return offset * 2;
}

void set_cursor_offset(u16int offset) {
    offset /= 2;
    
    port_outb(PORT_SCREEN_CTRL, 14);
    port_outb(PORT_SCREEN_DATA,offset >> 8);

    port_outb(PORT_SCREEN_CTRL, 15);
    port_outb(PORT_SCREEN_DATA, offset & 0xff);
}

static u16int cal_offset(u16int row, u16int col) { return 2*(row*MAX_COL + col); }
static u16int cal_row(u16int offset) { return  (offset / (2*MAX_COL)); }
static u16int cal_col(u16int offset) { return  offset/2 - (cal_row(offset)*MAX_COL); }
