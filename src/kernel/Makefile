.PHONY: all tmp

#Dirs
BUILD?=/build
KERNEL_BUILD?=/

INC=inc

SRC_DIRS=$(shell find ./* -type d)
TMP_DIRS=$(patsubst ./%, $(BUILD)/%, $(SRC_DIRS))

#Tools
GCC=gcc #/usr/local/x86_64elfgcc/bin/x86_64-elf-gcc
GCCFLAGS=-I$(INC)/ -ffreestanding -m64 -g -nostdlib -Wall -Wextra -mcmodel=kernel -mno-red-zone -fno-common -fno-pic -fno-stack-protector
LD=ld #/usr/local/x86_64elfgcc/bin/x86_64-elf-ld
NASM=nasm

#Misc
ASM_SOURCE=$(shell find . -name "*.asm")
ASM_OBJ=$(patsubst ./%.asm, $(BUILD)/%.asm.o, $(ASM_SOURCE))

C_SOURCE=$(shell find . -name "*.c")
C_OBJ=$(patsubst ./%.c, $(BUILD)/%.c.o, $(C_SOURCE))

tmp: 
	@mkdir -p $(TMP_DIRS) # create all tmp dirs

all: tmp $(KERNEL_BUILD)

$(BUILD)/%.asm.o: %.asm
	@echo "* NASM * $<"
	@$(NASM) -f elf64 $< -o $@
$(BUILD)/%.c.o: %.c
	@echo "* GCC *  $<"
	@$(GCC) $(GCCFLAGS) $< -o $@ -c
$(KERNEL_BUILD): $(ASM_OBJ) $(C_OBJ)
	@$(LD) -o $@ $^ -T link.ld -nostdlib -z max-page-size=4096