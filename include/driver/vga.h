#ifndef VGA_H
#define VGA_H
#include "types.h"
#define VGA_ADDRESS 0xB8000;
#define MAX_COL 80
#define MAX_ROW 25
#define VGA_MAX_OFFSET MAX_COL*MAX_ROW*2

void clear_screen();

u16int vga_scroll();

void printk(char* str);
void printkc(char *str, u8int _color);

u16int get_cursor_offset();

void set_cursor_offset(u16int offset); 

static u16int cal_offset(u16int row, u16int col);
static u16int cal_row(u16int offset);
static u16int cal_col(u16int offset);
#endif
