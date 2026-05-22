#include <fb.h>
#include <debug.h>
#include <boot.h>
#include <limine.h>
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

void fbInit() {
    struct limine_framebuffer *fb = limineFbRequest().response->framebuffers[0];

    addr = (u64)fb->address;
    x    = (u64)fb->pitch;
    y    = (u64)fb->height;
    
    debug("fb: PITCH:%d HEIGHT:%d ADDR:%x\n",x,y,addr);

}