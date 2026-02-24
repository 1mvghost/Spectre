#include <debug.h>
#include <stdarg.h>
#define COM1 0x3F8

int debugEmpty() {
    return in8(COM1+5) & 0x20;
}
void debugPutc(char c) {
    while(debugEmpty() == 0) {}

    out8(COM1,c);
}
void debugPuts(char* s) {
    while(*s) {
        debugPutc(*s);
        s++;
    }
}
void debugPutNum(u64 num, int radix) {
    if(!num) {
        debugPutc('0'); 
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
        debugPutc(buf[pos]);
    }
    
}
void debug(char* fmt, ...) {
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
                        debugPutc(*fmt);
                        ++fmt;
                        break;
                }
                break;
            case 1:
                switch (*fmt){
                    case 'c':
                        //putc(va_arg(va,char))
                        debugPutc((char)va_arg(va,char*));
                        break;
                    case 's':
                        debugPuts(va_arg(va,char*));
                        break;
                    case 'd':
                        debugPutNum(va_arg(va,u64),10);
                        break;
                    case 'x':
                        debugPutNum(va_arg(va,u64),16);
                        break;
                    case '%':
                        debugPutc('%');
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

void debugInit() {
    out8(COM1 + 1, 0x00);
    out8(COM1 + 3, 0x80);
    out8(COM1 + 0, 0x03);
    out8(COM1 + 1, 0x00);
    out8(COM1 + 3, 0x03);
    out8(COM1 + 2, 0xC7);
    out8(COM1 + 4, 0x0B);
    out8(COM1 + 4, 0x1E);
    out8(COM1 + 0, 0xAE); 

   if(in8(COM1 + 0) != 0xAE) {
      return 1;
   }
   out8(COM1 + 4, 0x0F);

}