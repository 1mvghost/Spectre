#include <font.h>
#include <fb.h>

void fontDraw(u64 x, u64 y) {
    u64 loc = (y*(fbResX()/4)) + x; 
    *((u32*)fbGetAddr()+loc) = 0b00000000111111111111111111111111;
}

void fontCh(u64 x, u64 y, char c) {
    u8* glyph = (u8*)font+(c*FHEIGHT);
    for(int yy=0; yy<FHEIGHT; yy++) {
        for(int xx=0; xx<FWIDTH; xx++) {
            if(glyph[yy]>>(FWIDTH-1-xx) & 1) {
                fontDraw(x+xx,y+yy);
            }
        }
    }
}