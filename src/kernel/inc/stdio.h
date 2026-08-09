#ifndef STDIO_H
#define STDIO_H

#define INFO 1
#define ERR 2
#define PANIC 3

#include <util.h>
#include <stdarg.h>

void printf(int id, char* fmt, ...);
void sprintf(u8* buf, char *fmt, ...);
void vsprintf(u8* buf, char *fmt, va_list va);
#endif