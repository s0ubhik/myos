#ifndef STRING_H
#define STRING_H
#include "types.h"

void int_to_ascii(int n, char str[]);

unsigned int strlen(char s[]);
void strcat(char str1[], char str2[]);
void print_hex(u32int n, u8int c);
void int_to_hex(unsigned long num, char str[], u8int c);

void str_reverse(char s[]);

void backspace(char s[]);

void append(char s[], char n);
void strend(char str[]);

int strcmp(char s1[], char s2[]);

#endif