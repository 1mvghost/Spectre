#include <stdio.h>
#include <stdarg.h>
#include <font.h>
#include <fb.h>
#include <debug.h>

static u64 curX=0;
static u64 curY=0;

void scroll(){
    /* terrible scroll */
    u64 l = (fbResX()*16)*((fbResY()/16)-1);
    memmove(fbGetAddr(), fbGetAddr()+(fbResX()*16), l);
    memset(fbGetAddr()+l, 0, fbResX()*16);
    curY=fbResY()-16;
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
int sputc(u8* buf, int i, char ch) {
    buf[i] = ch;
    i++;
    return i;
}
int sputNum(u8* buf, int i, u64 num, int radix) {
    if(!num) {
        sputc(buf,i,'0'); 
        return;
    }
    char nbuf[32];
    char* hex="0123456789ABCDEF";
    int pos = 0;
    while(num > 0) {
        nbuf[pos] = hex[num % radix];
        num /= radix;
        pos++;
    }

    while (pos-- > 0) {
        i=sputc(buf,i,nbuf[pos]);
    }
    return i;
    
}
void puts(char* str){
    while(*str){
        putc(*str);
        ++str;
    }
}
int sputs(u8* buf, int i, char* str) {
    while(*str){
        sputc(buf,i,*str);
        ++i;
        ++str;
    }
    return i;
}


void vsprintf(u8* buf, char *fmt, va_list va) {
    int state = 0;
    int i = 0;
    while(*fmt) {
        switch (state){
            case 0:
                switch (*fmt){
                    case '%':
                        state = 1;
                        ++fmt;
                        break;
                    default:
                        i=sputc(buf,i,*fmt);
                        ++fmt;
                        break;
                }
                break;
            case 1:
                switch (*fmt){
                    case 'c':
                        //putc(va_arg(va,char))
                        i=sputc(buf, i, (char)va_arg(va,char*));
                        break;
                    case 's':
                        i=sputs(buf, i, va_arg(va,char*));
                        break;
                    case 'd':
                        i=sputNum(buf,i,va_arg(va,u64),10);
                        break;
                    case 'x':
                        i=sputNum(buf,i,va_arg(va,u64),16);
                        break;
                    case '%':
                        i=sputc(buf,i,'%');
                        break;
                    default:
                        break;
                }
                state = 0;
                fmt++;
        }
    }
}
void sprintf(u8* buf, char *fmt, ...) {
    va_list va;
    va_start(va,fmt);

    vsprintf(buf,fmt,va);

    va_end(va);
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
    va_list va;
    va_start(va,fmt);

    u8 buf[1024];
    memset(buf,0,1024);
    vsprintf(buf,fmt,va);

    for(int i = 0; i < 1024; i++) {
        if(!buf[i]) break;
        putc(buf[i]);
        debugPutc(buf[i]);
    }

    va_end(va);
}
