#ifndef ACPI_H
#define ACPI_H

#include <util.h>

typedef struct {
   char signature[8];
   u8 checksum;
   char oemId[6];
   u8 revision;
   u32 rsdt;
} __attribute__((packed)) RSDP;

void acpiInit(u64 rsdpAddr);
void acpiShutdown();

#endif