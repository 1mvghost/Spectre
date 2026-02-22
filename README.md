# * SPECTRE *
Spectre is a 64-Bit Hobby Kernel made in C. It uses [Limine](https://github.com/limine-bootloader/limine) for booting.

# * FEATURES *
* Custom Console <br/>
* Disk (AHCI & IDE) drivers <br/>
* ACPI <br/>

# * UPCOMING FEATURES *
* VFS <br/>
* User Mode <br/>
* Multitasking <br/>
* SMP </br>
* ..and much more</br>

# * COMPILING *
(no need to compile, release is in the "Releases" tab)
1. Clone the repository
```bash
git clone https://github.com/1mvghost/Spectre.git
```
2. Make (you may need some dependencies installed like x86_64-elf-gcc, nasm etc)
```bash
make all
```
3. You can now run the OS with either of these commands :)
```bash
./qemu.sh
./qemuUefi.sh
./bochs.sh
```

