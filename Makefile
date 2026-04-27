#Dirs
SRC=src
BUILD=build
ISO=iso
LIMINE=limine

KERNEL_SRC=$(SRC)/kernel

#Misc
KERNEL_BUILD=$(ISO)/boot/kernel.sys
KERNEL_INC=$(KERNEL_SRC)/inc
all: clean
	mkdir -p $(BUILD)
	make -C $(KERNEL_SRC) BUILD=$(abspath $(BUILD)) INC=$(abspath $(KERNEL_INC)) ISO=$(abspath $(ISO)) all

	xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        $(ISO)/ -o $(BUILD)/os.iso
	
	./$(LIMINE)/limine bios-install $(BUILD)/os.iso
clean:
	rm -rf ./$(BUILD)/*
	rm -rf $(KERNEL_BUILD)