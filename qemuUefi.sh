unset GTK_PATH 
qemu-system-x86_64 -cdrom ./build/os.iso -cpu qemu64 -boot order=d -bios /usr/share/ovmf/OVMF.fd -net none
# -device ahci,id=ahci -device ide-hd,drive=disk,bus=ahci.0 -drive file=fat32.img,if=none,id=disk 