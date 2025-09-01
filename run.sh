#!/bin/bash
set -e

rm -rf Binaries
mkdir -p Binaries

nasm bootloader/boot.asm -f bin -o Binaries/bootloader.bin

i386-elf-gcc -m32 -ffreestanding -fno-pie -fno-stack-protector -O2 \
  -Wall -Wextra -c kernel/kernel.c -o Binaries/kernel.o

i386-elf-ld -nostdlib -T linker.ld -z max-page-size=0x1000 \
  -o Binaries/kernel.elf Binaries/kernel.o

i386-elf-objcopy -O binary Binaries/kernel.elf Binaries/kernel.bin

dd if=/dev/zero of=Binaries/disk.img bs=512 count=4096
dd if=Binaries/bootloader.bin of=Binaries/disk.img bs=512 conv=notrunc
dd if=Binaries/kernel.bin     of=Binaries/disk.img bs=512 seek=1 conv=notrunc

# Optionnel: voir la taille
stat -c "kernel.bin: %s bytes" Binaries/kernel.bin

# QEMU en mode “no reboot” pour diagnostiquer une triple fault:
LD_PRELOAD=/lib/x86_64-linux-gnu/libpthread.so.0 LD_LIBRARY_PATH=/lib/x86_64-linux-gnu:/usr/lib/x86_64-linux-gnu qemu-system-x86_64 -drive format=raw,file=Binaries/disk.img -m 128M -no-reboot
