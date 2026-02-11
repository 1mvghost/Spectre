#ifndef PCI_H
#define PCI_H

#include <util.h>

void pciOut8(u32 bus, u32 dev, u32 func, u32 offset, u8 val);
void pciOut32(u32 bus, u32 dev, u32 func, u32 offset, u32 val);
void pciInit();

#endif