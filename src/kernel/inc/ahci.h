#ifndef AHCI_H
#define AHCI_H
#include <util.h>

typedef volatile struct {
    u32 Clb;
    u32 ClbUp;
    u32 Fb;
    u32 FbUp;
    u32 Is;
    u32 Ie;
    u32 Cmd;
    u32 Reserved;
    u32 Tfd;
    u32 Sign;
    u32 Ssts;
    u32 SCtrl;
    u32 SErr;
    u32 SAct;
    u32 Ci;
    u32 Sntf;
    u32 Fbs;
    u32 Reserved1[11];
    u32 Vendor[4];
} HbaPort;

bool ahciRead(int p, u64 lba, u32 sectAmount, u16 *buf);
bool ahciWrite(int p, u64 lba, u32 sectAmount, u16 *buf);
void ahciInit(u32 bar5);

#endif