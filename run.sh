#!/bin/bash

rm -rf Binaries
mkdir -p Binaries

gcc -m32 -ffreestanding -fno-pie -nostdlib -c kernel/kernel.c -o Binaries/kernel.o
ld -m elf_i386 -T linker.ld -o Binaries/kernel.elf Binaries/kernel.o
objcopy -O binary Binaries/kernel.elf Binaries/kernel.bin

nasm -f bin bootloader/boot.asm -o Binaries/bootloader.bin
cat Binaries/bootloader.bin Binaries/kernel.bin > Binaries/os.bin

qemu-system-x86_64 -drive format=raw,file=Binaries/os.bin