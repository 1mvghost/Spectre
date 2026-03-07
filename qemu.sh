unset GTK_PATH
#qemu-system-x86_64 -cdrom ./build/os.iso -m 2048 -M q35 -vga vmware -serial stdio -enable-kvm
#qemu-system-x86_64 -cdrom ./build/os.iso -device ide-hd,drive=disk,bus=ide.0 -drive file=testimg.img,if=none,id=disk -m 8192 -M pc -vga vmware -serial stdio -enable-kvm 
qemu-system-x86_64 -cdrom ./build/os.iso -m 8G -M q35 -vga vmware -serial stdio -boot order=d -accel kvm