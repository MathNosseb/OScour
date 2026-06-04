#!/bin/bash

echo Supression des ancien fichiers....
rm -rf Binaries
mkdir -p Binaries

echo Compilation du kernel....
gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/kernel.c -o Binaries/kernel.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/output/vga.c    -o Binaries/vga.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/sys/struct.c    -o Binaries/struct.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/sys/string.c    -o Binaries/string.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/sys/hex.c    -o Binaries/hex.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/sys/list.c    -o Binaries/list.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/input/keyboard.c    -o Binaries/keyboard.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/app/shell.c    -o Binaries/shell.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/mem/mem.c    -o Binaries/mem.o
gcc -m32 -ffreestanding -fno-pie -nostdlib -O0 -c kernel/app/programs.c    -o Binaries/programs.o

echo linkage....
ld -m elf_i386 -T linker.ld -o Binaries/kernel.elf \
    Binaries/kernel.o \
    Binaries/vga.o \
    Binaries/struct.o \
    Binaries/string.o \
    Binaries/hex.o \
    Binaries/keyboard.o \
    Binaries/shell.o \
    Binaries/mem.o \
    Binaries/programs.o \
    Binaries/list.o

echo Envoie kernel vers binaire....
objcopy -O binary Binaries/kernel.elf Binaries/kernel.bin

echo Compilation bootloader....
nasm -f bin bootloader/boot.asm -o Binaries/bootloader.bin

echo Compilation des programmes externes....
python3 program_compiler.py

echo affichage taille des programmes....
wc -c Binaries/compiledPrograms.bin

echo truncate + ajout octets au kernel....
truncate -s %512 Binaries/kernel.bin
dd if=/dev/zero bs=512 count=1 >> Binaries/kernel.bin

echo ajout des programmes externes au kernel....
cat out.bin >> Binaries/kernel.bin

echo ajout du kernel et du bootloader a l os....
cat Binaries/bootloader.bin Binaries/kernel.bin > Binaries/os.bin

echo derniere pass de truncate sur l os....
truncate -s %512 Binaries/os.bin
#VBoxManage convertfromraw Binaries/os.bin Binaries/os.vdi --format VDI

echo lancement calculs info OS....
size=$(wc -c < Binaries/kernel.bin)
echo $size octets de kernel

secteurs=$(( ($(wc -c < Binaries/kernel.bin) + 511) / 512 ))
echo $secteurs secteurs a lire

end=$((0x1000 + size))
printf "kernel end = 0x%X\n" $end

stack_size=$((0x80000 - end))
printf "stack size = 0x%X\n" $stack_size
echo $((stack_size)) Octets, $((stack_size/1000)) Ko, $((stack_size/1000/1000)), Mo

printf "\\x$(printf '%02x' "$secteurs")" | dd of=Binaries/os.bin bs=1 seek=167 conv=notrunc
xxd Binaries/os.bin > os.hex

qemu-system-x86_64 -enable-kvm -cpu host -m 1G -drive format=raw,file=Binaries/os.bin