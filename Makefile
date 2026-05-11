.PHONY: all clean limine iso

#Dirs
SRC=src
BUILD=build
ISO=iso
LIMINE=limine

KERNEL_SRC=$(SRC)/kernel

#Misc
KERNEL_BUILD=kernel.sys
ISO_BUILD	=$(BUILD)/os.iso


$(BUILD):
	@mkdir -p $(BUILD)

all: clean kernel limine iso

kernel: $(BUILD)
	@make -C $(KERNEL_SRC) BUILD=$(abspath $(BUILD)) KERNEL_BUILD=$(abspath $(KERNEL_BUILD)) all

limine:
	@make -C $(LIMINE) all

iso: $(BUILD)
	@mkdir -p $(ISO)/boot/limine/ $(ISO)/EFI/BOOT

	@cp $(LIMINE)/limine-bios-cd.bin $(LIMINE)/limine-bios.sys $(LIMINE)/limine-uefi-cd.bin $(ISO)/boot/limine/
	@cp $(LIMINE)/BOOTX64.EFI $(LIMINE)/BOOTIA32.EFI $(ISO)/EFI/BOOT/
	@cp limine.conf $(ISO)/boot/limine/limine.conf

	@cp $(KERNEL_BUILD) $(ISO)/boot/kernel.sys

	@xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
        -no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        -apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
        -efi-boot-part --efi-boot-image --protective-msdos-label \
        $(ISO)/ -o $(ISO_BUILD)
	
	@$(LIMINE)/limine bios-install $(ISO_BUILD)

clean:
	rm -rf $(BUILD) $(ISO) $(KERNEL_BUILD)