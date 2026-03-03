#ifndef FB_H
#define FB_H
#include <util.h>
void fbInit(u64 resX, u64 resY, u64 fbAddr);

u64 fbResX();
u64 fbResY();
u64 fbGetAddr();
#endif