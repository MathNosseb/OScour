read -p "push? (y/n) " query
if [ "$query" == "y" ]; then
    git add .
    read -p "Enter commit message: " commit
    git commit -m "$commit"
    git push
fi

read -p "run script?    (y/n)   " query
if [ "$query" == "y" ]; then

    export PATH=$PATH:/usr/local/i386elfgcc/bin

    nasm "bootloader/boot.asm" -f bin -o "Binaries/boot.bin"
    nasm "bootloader/kernel_entry.asm" -f elf -o "Binaries/kernel_entry.o"
    i386-elf-gcc -ffreestanding -m32 -g -c "Kernel/kernel.c" -o "Binaries/kernel.o"
    i386-elf-gcc -ffreestanding -m32 -g -c "Kernel/fprint.c" -o "Binaries/fprint.o"

    nasm "bootloader/zeroes.asm" -f bin -o "Binaries/zeroes.bin"

    i386-elf-ld -o "Binaries/full_kernel.bin" -Ttext 0x1000 "Binaries/kernel_entry.o" "Binaries/fprint.o" "Binaries/kernel.o" --oformat binary

    cat "Binaries/boot.bin" "Binaries/full_kernel.bin" "Binaries/zeroes.bin"  > "Binaries/OS.bin"

    qemu-system-x86_64 -drive format=raw,file="Binaries/OS.bin",index=0,if=floppy,  -m 128M
fi
