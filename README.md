# * SPECTRE *
Spectre is a 64-Bit Hobby Kernel made in C. It uses [Limine](https://github.com/limine-bootloader/limine) for booting.
# * FEATURES *
* AHCI <br/>
* ACPI <br/>
* PMM <br/>
* Basic Heap Allocator <br/>
* VFS <br/>
* SMP </br>

# * UPCOMING FEATURES *
* User Mode <br/>
* Multitasking <br/>
* ..and much more</br>

# * COMPILING *
(no need to compile, release is in the "Releases" tab)
1. Clone the repository
```bash
git clone https://github.com/1mvghost/Spectre.git
```
2. Install some stuff you may need and make (make sure you're at the root!)
```bash
./install.sh
make all
```
3. You can now run the OS with either of these commands :)
```bash
./qemu.sh
./qemuUefi.sh
./bochs.sh
```

