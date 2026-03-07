unset GTK_PATH 
qemu-system-x86_64 -cdrom ./build/os.iso -cpu qemu64 -boot order=d -bios /usr/share/ovmf/OVMF.fd -net none -serial stdio