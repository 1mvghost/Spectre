#include <font.h>

static u64 fbAddr;
static u64 resPitch;

u64 fontPitch(){
    return resPitch;
}
void fontDraw(u64 x, u64 y) {
    u64 loc = (y*(resPitch/4)) + x; 
    *((u32*)fbAddr+loc) = 0b00000000111111111111111111111111;
}

void fontCh(u64 x, u64 y, char c) {
    u8* glyph = (u8*)font+(c*16);
    for(int yy=0; yy<16; yy++) {
        for(int xx=0; xx<8; xx++) {
            if(glyph[yy]>>(7-xx) & 1) {
                fontDraw(x+xx,y+yy);
            }
        }
    }
}
void fontInit(u64 addr, u64 pitch) {
    fbAddr=addr;
    resPitch=pitch;
}