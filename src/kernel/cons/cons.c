#include <fb.h>
#include <font.h>
#include <mem.h>
#include <debug.h>

static u64 curX=0;
static u64 curY=0;

static u64 width=0;
static u64 height=0;

void consScroll(){
    u64 row = height / FHEIGHT;
    u64 r   = fbResX() * FHEIGHT;

    memmove((u8*)fbGetAddr(), (u8*)fbGetAddr()+r, (r*row)-r);
    memset((u8*)(fbGetAddr()+(row-1)*r), 0, r);
    
    curY-=FHEIGHT;
    curX=0;
}

void consNewline(){
    curY+=FHEIGHT;
    curX=0;

    if(curY >= height){
        consScroll();
    }
}

Splock putcSp = ATOMIC_FLAG_INIT; 
void consPutc(char ch) {
    mSpinlockAcquire(&putcSp);
    switch (ch) {
        case '\n':
            consNewline();
            break;
        default:
            if(curX>=width) {
                consNewline();
            }
            fontCh(curX,curY,ch);
            curX+=FWIDTH;
            break;       
    }
    debugPutc(ch);
    mSpinlockDrop(&putcSp);
}
void consInit() {
    width  = ALIGN_DOWN(fbResX()/4, FWIDTH);
    height = ALIGN_DOWN(fbResY(),   FHEIGHT);

}