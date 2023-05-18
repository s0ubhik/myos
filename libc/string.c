#include "libc/string.h"
#include "types.h"
void int_to_ascii(int n, char str[]) {
    int i, sign;
    if ((sign = n) < 0) n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';
    str[i] = '\0';
    str_reverse(str);
}

void int_to_bin(u32int n, char str[]){
    u8int i = 0;
    while (n > 0){
        str[i++] = n % 2 & 1 ? '1' : '0';
        n /= 2;
    }
    str[i] = '\0';
    str_reverse(str);
}

unsigned int strlen(char s[]){
    unsigned int i = 0;
    while (s[i] != '\0') ++i;
    return i;
}

void str_reverse(char s[]){
    char c;
    unsigned int i, j, max;
    max = strlen(s);
    for (i=0, j = max-1; i < max/2; i++, j = max - i - 1){
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void int_to_hex(unsigned long num, char str[])
{
    int i = 0;
    while( num > 0) {
        str[i++] = "0123456789ABCDEF"[num % 16];
        num = num/16;
    };

    str[i] = '\0';
    str_reverse(str);
}

void backspace(char s[]){
    int len = strlen(s);
    s[len-1] = '\0';
}

void append(char s[], char n) {
    int len = strlen(s);
    s[len] = n;
    s[len+1] = '\0';
}

int strcmp(char s1[], char s2[]){
    for (unsigned int i=0; s1[i] == s2[i]; i++){
        if (s1[i] == '\0') return 0;
    }
    return s1-s2;
}

