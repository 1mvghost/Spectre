#ifndef STDIO_H
#define STDIO_H
#include <util.h>
#include <stdarg.h>
void printf(int id, char* fmt, ...);
void sprintf(u8* buf, char *fmt, ...);
void vsprintf(u8* buf, char *fmt, va_list va);
#endif