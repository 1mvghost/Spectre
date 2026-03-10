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
void debug(char* fmt, ...) {
    va_list va;
    va_start(va,fmt);

    u8 buf[1024];
    memset(buf,0,1024);
    vsprintf(buf,fmt,va);

    for(int i = 0; i < 1024; i++) {
        if(!buf[i]) break;
        debugPutc(buf[i]);
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