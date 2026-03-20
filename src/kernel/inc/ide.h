#ifndef IDE_H
#define IDE_H

#include <util.h>
void ideRead(u8 disk, u32 lba, u8 sectAmount, u16* buf);
void ideInit(u32 bar0, u32 bar1, u32 bar2, u32 bar3, u32 bar4);
#endif