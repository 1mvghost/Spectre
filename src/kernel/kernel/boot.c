#include <boot.h>
#include <limine.h>
#include <util.h>

__attribute__((
    used,
    section(
        ".limine_requests"))) static volatile struct limine_framebuffer_request
    framebufferRequest = {.id = LIMINE_FRAMEBUFFER_REQUEST_ID, .revision = 0};

__attribute__((
    used,
    section(".limine_requests"))) static volatile struct limine_memmap_request
    mMapRequest = {.id = LIMINE_MEMMAP_REQUEST_ID, .revision = 0};
__attribute__((
    used,
    section(".limine_requests"))) static volatile struct limine_rsdp_request
    rsdpRequest = {.id = LIMINE_RSDP_REQUEST_ID, .revision = 4};

__attribute__((used, section(".limine_requests"))) static volatile struct
    limine_firmware_type_request firmwareRequest = {
        .id = LIMINE_FIRMWARE_TYPE_REQUEST_ID,
        .revision = 4};
__attribute__((
    used,
    section(".limine_requests"))) static volatile struct limine_module_request
    modRequest = {.id = LIMINE_MODULE_REQUEST_ID, .revision = 4};
__attribute__((
    used,
    section(".limine_requests"))) static volatile struct limine_mp_request
    mpRequest = {.id = LIMINE_MP_REQUEST_ID, .revision = 4};

__attribute__((used, section(".limine_requests_start"))) static volatile u64
    limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end"))) static volatile u64
    limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

struct limine_framebuffer_request limineFbRequest() {
  return framebufferRequest;
}
struct limine_memmap_request limineMMapRequest() {
  return mMapRequest;
}
struct limine_rsdp_request limineRsdpRequest() {
  return rsdpRequest;
}
struct limine_firmware_type_request limineFirmwareRequest() {
  return firmwareRequest;
}
struct limine_module_request limineModRequest() {
  return modRequest;
}
struct limine_mp_request limineMpRequest() {
  return mpRequest;
}