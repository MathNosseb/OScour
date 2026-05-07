#!/bin/bash

rm -rf Binaries
mkdir -p Binaries

gcc -m32 -ffreestanding -fno-pie -nostdlib -c kernel/kernel.c -o Binaries/kernel.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -c kernel/vga.c    -o Binaries/vga.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -c kernel/struct.c    -o Binaries/struct.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -c kernel/keyboard.c    -o Binaries/keyboard.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -c kernel/shell.c    -o Binaries/shell.o

ld -m elf_i386 -T linker.ld -o Binaries/kernel.elf \
    Binaries/kernel.o \
    Binaries/vga.o \
    Binaries/struct.o \
    Binaries/keyboard.o \
    Binaries/shell.o


objcopy -O binary Binaries/kernel.elf Binaries/kernel.bin

nasm -f bin bootloader/boot.asm -o Binaries/bootloader.bin
cat Binaries/bootloader.bin Binaries/kernel.bin > Binaries/os.bin

truncate -s %512 Binaries/os.bin
VBoxManage convertfromraw Binaries/os.bin Binaries/os.vdi --format VDI

qemu-system-x86_64 -drive format=raw,file=Binaries/os.bin