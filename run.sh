#!/bin/bash

rm -rf Binaries
mkdir -p Binaries

gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/kernel.c -o Binaries/kernel.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/output/vga.c    -o Binaries/vga.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/sys/struct.c    -o Binaries/struct.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/sys/string.c    -o Binaries/string.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/sys/hex.c    -o Binaries/hex.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/input/keyboard.c    -o Binaries/keyboard.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/app/shell.c    -o Binaries/shell.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/mem/mem.c    -o Binaries/mem.o

ld -m elf_i386 -T linker.ld -o Binaries/kernel.elf \
    Binaries/kernel.o \
    Binaries/vga.o \
    Binaries/struct.o \
    Binaries/string.o \
    Binaries/hex.o \
    Binaries/keyboard.o \
    Binaries/shell.o \
    Binaries/mem.o


objcopy -O binary Binaries/kernel.elf Binaries/kernel.bin

nasm -f bin bootloader/boot.asm -o Binaries/bootloader.bin
cat Binaries/bootloader.bin Binaries/kernel.bin > Binaries/os.bin

truncate -s %512 Binaries/os.bin
VBoxManage convertfromraw Binaries/os.bin Binaries/os.vdi --format VDI

size=$(wc -c < Binaries/kernel.bin)
echo $size octets de kernel

secteurs=$(( ($(wc -c < Binaries/kernel.bin) + 511) / 512 ))
echo $secteurs secteurs a lire

end=$((0x1000 + size))
printf "kernel end = 0x%X\n" $end

stack_size=$((0x80000 - end))
printf "stack size = 0x%X\n" $stack_size
echo $((stack_size)) Octets, $((stack_size/1000)) Ko, $((stack_size/1000/1000)), Mo

xxd Binaries/os.bin > os.hex

qemu-system-x86_64 -m 10M -drive format=raw,file=Binaries/os.bin