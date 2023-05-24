#include "string.h"

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

void strcat(char str1[], char* str2){
    unsigned int i = 0;
    unsigned int cur = strlen(str1);
    unsigned int m = strlen(str2);

    while(str2[i] != '\0'){
        str1[cur+i] = str2[i];
        i++;
    }
    str1[cur+i] = '\0';
}

void strend(char str[]){
    str[strlen(str)] = '\0';
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
void print_hex(u32int n, u8int c){
  char s[c];
  int_to_hex(n, s, c);
  printk(s);
}

void int_to_hex(unsigned long num, char str[], u8int c)
{
    int i = 0;

    for (u8int m =0; m < c; m++)
        str[m] = '0';
    
    while( num > 0) {
        str[i++] = "0123456789ABCDEF"[num % 16];
        num = num/16;
    };

    str[c] = '\0';
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

