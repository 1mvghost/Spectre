#Dirs
BUILD?=/build
INC?=/inc
ISO?=/iso
SRC_DIRS=$(BUILD)/driver/ $(BUILD)/driver/disk $(BUILD)/cpu $(BUILD)/kernel $(BUILD)/cons $(BUILD)/mem $(BUILD)/util $(BUILD)/fs
#Tools
GCC=gcc #/usr/local/x86_64elfgcc/bin/x86_64-elf-gcc
GCCFLAGS=-I$(INC)/ -ffreestanding -m64 -g -nostdlib -Wall -Wextra -mcmodel=kernel -mno-red-zone -fno-common -fno-pic
LD=ld #/usr/local/x86_64elfgcc/bin/x86_64-elf-ld
NASM=nasm

#Misc
KERNEL_BUILD=$(ISO)/boot/kernel.sys

ASM_SOURCE=$(wildcard ./*.asm) $(wildcard ./*/*.asm) $(wildcard ./*/*/*.asm)
ASM_OBJ=$(patsubst ./%.asm, $(BUILD)/%.asm.o, $(ASM_SOURCE))

C_SOURCE=$(wildcard ./*.c) $(wildcard ./*/*.c) $(wildcard ./*/*/*.c)
C_OBJ=$(patsubst ./%.c, $(BUILD)/%.c.o, $(C_SOURCE))

tmp: 
	mkdir $(SRC_DIRS) # create all tmp dirs
all: tmp $(KERNEL_BUILD)

$(BUILD)/%.asm.o: %.asm
	$(NASM) -f elf64 $< -o $@

$(BUILD)/%.c.o: %.c
	$(GCC) $(GCCFLAGS) $< -o $@ -c

$(KERNEL_BUILD): $(ASM_OBJ) $(C_OBJ)
	$(LD) -o $@ $^ -T link.ld -nostdlib -z max-page-size=4096