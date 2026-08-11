.PHONY: all tmp

#Dirs

BUILD?=./build
KERNEL_BUILD?=./

INC=inc

SRC_DIRS=$(shell find ./* -type d)
TMP_DIRS=$(patsubst ./%, $(BUILD)/%, $(SRC_DIRS))

#Tools

C=gcc #/usr/local/x86_64elfgcc/bin/x86_64-elf-gcc
C_FLAGS=-I$(INC)/ -ffreestanding -m64 -g -nostdlib -Wall -Wextra -mcmodel=kernel -mno-red-zone -fno-common -fno-pic -fno-stack-protector

LD=ld #/usr/local/x86_64elfgcc/bin/x86_64-elf-ld
LD_FLAGS=-T link.ld -nostdlib -z max-page-size=4096

ASM=nasm
ASM_FLAGS=-f elf64

#Misc

ASM_SOURCE=$(shell find . -name "*.asm")
ASM_OBJ=$(patsubst ./%.asm, $(BUILD)/%.asm.o, $(ASM_SOURCE))

C_SOURCE=$(shell find . -name "*.c")
C_OBJ=$(patsubst ./%.c, $(BUILD)/%.c.o, $(C_SOURCE))

tmp: 
	@mkdir -p $(TMP_DIRS) # create all tmp dirs

all: tmp $(KERNEL_BUILD)

$(BUILD)/%.asm.o: %.asm
	@echo "* asm: $<"
	@$(ASM) $(ASM_FLAGS) $< -o $@

$(BUILD)/%.c.o: %.c
	@echo "* c:   $<"
	@$(C) $(C_FLAGS) $< -o $@ -c

$(KERNEL_BUILD): $(ASM_OBJ) $(C_OBJ)
	@$(LD) -o $@ $^ $(LD_FLAGS)