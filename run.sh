x86_64-elf-gcc -ffreestanding -m32 -g -c SRC/kernel.c -o Build/kernel.o
x86_64-elf-gcc -ffreestanding -m32 -g -c SRC/Hardware\ Files/ports.c -o Build/ports.o
x86_64-elf-gcc -ffreestanding -m32 -g -c SRC/Hardware\ Files/keyboard.c -o Build/keyboard.o
# ** MODIFIED: Added shell.c compilation **
x86_64-elf-gcc -ffreestanding -m32 -g -c SRC/Hardware\ Files/shell.c -o Build/shell.o
nasm SRC/Boot-Level\ Files/kernel_entry.asm -f elf -o Build/kernel_entry.o
# ** MODIFIED: Added Build/shell.o to linker **
x86_64-elf-ld -m elf_i386 -o Build/binaries/kernel.bin -Ttext 0x1000 Build/kernel_entry.o Build/kernel.o Build/ports.o Build/keyboard.o Build/shell.o --oformat binary
nasm SRC/Boot-Level\ Files/boot.asm -f bin -o Build/binaries/boot.bin
cat Build/binaries/boot.bin Build/binaries/kernel.bin > Build/binaries/rawOSfile.bin
cat Build/binaries/rawOSfile.bin Build/zero.bin > Build/binaries/withZeroes.bin
qemu-system-x86_64 -drive format=raw,file="Build/binaries/rawOSfile.bin",index=0,if=floppy -m 128M