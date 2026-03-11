#include <fb.h>
#include <debug.h>

static u64 x;
static u64 y;
static u64 addr;

u64 fbResX() {
    return x;
}
u64 fbResY() {
    return y;
}
u64 fbGetAddr() {
    return addr;
}

void fbInit(u64 resX, u64 resY, u64 fbAddr) {
    debug("fb: PITCH:%d HEIGHT:%d ADDR:%x\n",resX,resY,fbAddr);
    x=resX; y=resY; addr=fbAddr;

}