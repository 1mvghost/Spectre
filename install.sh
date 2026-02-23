sudo apt install nasm
sudo apt install qemu-system
sudo apt install xorriso
git clone https://codeberg.org/Limine/limine.git --branch=v10.x-binary --depth=1
make -C ./limine/ all