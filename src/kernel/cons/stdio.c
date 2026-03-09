#include <stdio.h>
#include <stdarg.h>
#include <font.h>
#include <fb.h>
u64 curX=0;
u64 curY=0;
void scroll(){
    /* terrible scroll */
    memmove(fbGetAddr(), fbGetAddr()+(fbResX()*16), (fbResX()*16)*47);
    memset(fbGetAddr()+((fbResX()*16)*47), 0, fbResX()*16);
    curY=768-16;
    curX=0;
}
void putc(char ch){
    switch (ch) {
        case '\n':
            curY+=16;
            curX=0;
            if(curY>=fbResY()){
                scroll();
            }
            break;
        default: 
            if(curX>(fbResX()/4)) {
                curX=0;
                curY+=16;
            }
            if(curY>=fbResY()) {
                scroll();
            }
            fontCh(curX,curY,ch);
            curX+=8;
            break;       
    }
}

void putNum(u64 num, int radix) {
    if(!num) {
        putc('0'); 
        return;
    }
    char buf[32];
    char* hex="0123456789ABCDEF";
    int pos = 0;
    while(num > 0) {
        buf[pos] = hex[num % radix];
        num /= radix;
        pos++;
    }

    while (pos-- > 0) {
        putc(buf[pos]);
    }
    
}

void puts(char* str){
    while(*str){
        putc(*str);
        ++str;
    }
}

void printf(int id, char* fmt, ...) {
    switch(id) {
        case 0:
            break;
        case 1:
            puts("* INFO *  ");
            break;
        case 2:
            puts("* ERROR * ");
            break;
        case 3:
            puts("* PANIC * ");
            break;
    }
    /* after 3h of debugging i finally got this working */
    va_list va;
    va_start(va,fmt);
    int state = 0;
    while(*fmt) {
        switch (state){
            case 0:
                switch (*fmt){
                    case '%':
                        state = 1;
                        ++fmt;
                        break;
                    default:
                        putc(*fmt);
                        ++fmt;
                        break;
                }
                break;
            case 1:
                switch (*fmt){
                    case 'c':
                        //putc(va_arg(va,char))
                        putc((char)va_arg(va,char*));
                        break;
                    case 's':
                        puts(va_arg(va,char*));
                        break;
                    case 'd':
                        putNum(va_arg(va,u64),10);
                        break;
                    case 'x':
                        putNum(va_arg(va,u64),16);
                        break;
                    case '%':
                        putc('%');
                        break;
                    default:
                        break;
                }
                state = 0;
                fmt++;
        }
    }
    va_end(va);
}