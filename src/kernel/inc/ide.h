#ifndef IDE_H
#define IDE_H

#include <util.h>
void ideRead(u8 disk, u8 sectAmount, u32 lba,u64 buf);
void ideInit(u32 bar0, u32 bar1, u32 bar2, u32 bar3, u32 bar4);
#endif