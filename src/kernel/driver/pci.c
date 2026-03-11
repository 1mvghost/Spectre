#include <pci.h>
#include <stdio.h>
#include <ide.h>
#include <ahci.h>
#include <vmm.h>
#include <debug.h>

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA    0xCFC

typedef struct {
    u16 Vendor;
    u16 Device;
    u16 Cmd;
    u16 Status;
    u8  RevisionId;
    u8  ProgIF;
    u8  Subclass;
    u8  Classcode;
    u8  CacheLineSize;
    u8  LatencyTimer;
    u8  Header;
    u8  Bist;
    u32 Bar0;
    u32 Bar1;
    u32 Bar2;
    u32 Bar3;
    u32 Bar4;
    u32 Bar5;
} PCIDevice;

static char* class[32] = {
    "Unknown",
    "Mass Storage Controller",
    "Network Controller",
    "Display Controller",
    "Multimedia Controller",
    "Memory Controller",
    "Bridge",
    "Simple Communication Controller",
    "Base System Peripheral",
    "Input Device Controller",
    "Docking Station",
    "Processor",
    "Serial Bus Controller",
    "Wireless Controller",
    "Intelligent Controller",
    "Satellite Communication Controller",
    "Encryption Controller",
    "Signal Processing Controller"
};

static int subClLen[32] = {
    0,/*0x0*/
    9,/*0x1*/
    1,/*0x2*/
    3,/*0x3*/
    4,/*0x4*/
    2,/*0x5*/
    11,/*0x6*/
    6,/*0x7*/
    7,/*0x8*/
    5,/*0x9*/
    1,/*0xA*/
    4,/*0xB*/
    10,/*0xC*/
    0,
    0,
    0,
    0,
    0
};
static char* subCl[32][32]={
    {},
    {"SCSI Bus Controller","IDE Controller","Floppy Controller","IPI Bus Controller","RAID Controller","ATA Controller","SATA Controller","Serial Attached SCSI Controller","Non-Volatile Memory Controller"},
    {"Ethernet Controller"},
    {"VGA Compatible Controller","XGA Controller","3D Controller"},
    {"Multimedia Video Controller","Multimedia Audio Controller","Computer Telephony Device","Audio Device"},
    {"RAM Controller", "Flash Controller"},
    {"Host Bridge","ISA Bridge","EISA Bridge","MCA Bridge","PCI-to-PCI Bridge","PCMCIA Bridge","NuBus Bridge","CardBus Bridge","RACEWay Bridge","PCI-to-PCI Bridge","InfiniBand-to-PCI Host Bridge"},
    {"Serial Controller", "Parallel Controller","Multiport Serial Controller","Modem","IEEE 488.1/2 Controller","Smart Card Controller"},
    {"PIC","DMA Controller","Timer","RTC Controller","PCI Hot-Plug Controller","SD Host Controller","IOMMU"},
    {"Keyboard Controller", "Digitizer Pen","Mouse Controller","Scanner Controller","Gameport Controller"},
    {"Generic"},
    {"386","486","Pentium","Pentium Pro"},
    {"FireWire Controller","ACCESS Bus Controller","SSA","USB Controller","Fibre Channel","SMBus Controller","InfiniBand Controller","IPMI Interface","SERCOS Interface","CANbus Controller"}
};
static PCIDevice pciDev[256];
static u64 devI=0;

char* pciGetVendor(u32 vendor) {
    switch(vendor) {
        case 0x8086: return "Intel Corporation";
        case 0x1022: return "AMD";
        case 0x10DE: return "NVIDIA Corporation";
        case 0x15AD: return "VMWare";
    }
    return "UNKNOWN";
}

u32 pciIn32(u32 bus, u32 dev, u32 func, u32 offset) {
    u32 address;
    address = (u32) (((u32)0x80000000) | (bus << 16) | (dev << 11) | (func << 8) | (offset & 0xFC));
    //printf(INFO,"%x,%x,%x,%x -> %x\n",bus,dev,func,offset,address);
    out32(CONFIG_ADDRESS, address);
    return in32(CONFIG_DATA);
}

u16 pciIn16(u32 bus, u32 dev, u32 func, u32 offset) {
    /*
    u32 address;
    u16 tmp = 0;
    address = (u32) (((u32)0x80000000) | (bus << 16) | (dev << 11) | (func << 8) | (offset & 0xFC));
    out32(CONFIG_ADDRESS, address);
    */
    u16 tmp = 0;
    tmp = (u16)((pciIn32(bus,dev,func,offset) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}
u8 pciIn8High(u32 bus, u32 dev, u32 func, u32 offset) {
    u16 tmp = pciIn16(bus,dev,func,offset);
    return (u8)(tmp >> 8);
}
u8 pciIn8Low(u32 bus, u32 dev, u32 func, u32 offset) {
    u16 tmp = pciIn16(bus,dev,func,offset);
    return (u8)(tmp & 0xFF);
}
void pciHandle(PCIDevice dev) {
    if(dev.Classcode==1 && dev.Subclass==1){
        /* IDE CONTROLLER */
        ideInit(dev.Bar0, dev.Bar1, dev.Bar2, dev.Bar3, dev.Bar4);
    }
    if(dev.Classcode==1 && dev.Subclass==6){
        /* AHCI */
        ahciInit(dev.Bar5);
    }
}
void pciCheckDevice(u32 bus, u32 dev) {
    for(int f = 0; f<8; f++) {
        u16 vendor =        pciIn16(bus,dev,f,0);
        /* 0xFFFF - NONEXISTENT DEVICE */
        if(vendor != 0xFFFF) {
            PCIDevice *d = &pciDev[devI];
            /*
            u16 device =        pciIn16(bus,dev,f,2);
            u16 cmd =           pciIn16(bus,dev,f,4);
            u16 status =        pciIn16(bus,dev,f,6);
            u8 revisionId =     pciIn8Low(bus,dev,f,8);
            u8 prog =           pciIn8High(bus,dev,f,8);
            u8 subclass =       pciIn8Low(bus,dev,f,10);
            u8 classcode =      pciIn8High(bus,dev,f,10);
            u8 header =         pciIn8Low(bus,dev,f,14);
            */
    
            d->Device =          pciIn16(bus,dev,f,2);
            d->Cmd =             pciIn16(bus,dev,f,4);
            d->Status =          pciIn16(bus,dev,f,6);
            d->RevisionId =      pciIn8Low(bus,dev,f,8);
            d->ProgIF =          pciIn8High(bus,dev,f,8);
            d->Subclass =        pciIn8Low(bus,dev,f,10);
            d->Classcode =       pciIn8High(bus,dev,f,10);
            d->CacheLineSize =   pciIn8Low(bus,dev,f,0xC);
            d->LatencyTimer =    pciIn8High(bus,dev,f,0xC);
            d->Header =          pciIn8Low(bus,dev,f,0xF);
            d->Bist =            pciIn8High(bus,dev,f,0xF);
            d->Bar0 =            pciIn32(bus,dev,f,0x10);
            d->Bar1 =            pciIn32(bus,dev,f,0x14);
            d->Bar2 =            pciIn32(bus,dev,f,0x18);
            d->Bar3 =            pciIn32(bus,dev,f,0x1C);
            d->Bar4 =            pciIn32(bus,dev,f,0x20);
            d->Bar5 =            pciIn32(bus,dev,f,0x24);
            
            char* subc = "Other";
            if(d->Subclass==0||d->Subclass<subClLen[d->Classcode]) {
                subc=subCl[d->Classcode][d->Subclass];
            }
            debug("pci: FOUND PCI: %s(%d) -> %s(%d) BAR0:%x BAR1:%x BAR2:%x BAR3:%x BAR4:%x BAR5:%x HEADER:%x [%s]\n",class[d->Classcode],d->Classcode,subc, d->Subclass,d->Bar0,d->Bar1,d->Bar2,d->Bar3,d->Bar4,d->Bar5,d->Header,pciGetVendor(vendor));
            pciHandle(*d);
            devI++;
        }
    }
}
void pciEnum() {
    u32 bus;
    u32 dev;

    for(bus = 0; bus<256; bus++) {
        for(dev = 0; dev<32; dev++) {
            pciCheckDevice(bus, dev);
        }
    }
}
void pciInit() {
    pciEnum();
}