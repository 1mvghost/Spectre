#ifndef BOOT_H
#define BOOT_H

#include <limine.h>

struct limine_framebuffer_request limineFbRequest();
struct limine_memmap_request limineMMapRequest();
struct limine_rsdp_request limineRsdpRequest();
struct limine_firmware_type_request limineFirmwareRequest();
struct limine_module_request limineModRequest();
struct limine_mp_request limineMpRequest();

#endif