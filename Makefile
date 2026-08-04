CC = gcc
ASM = nasm
SOURCES = $(shell find . -name "*.c")
OBJS = $(SOURCES:.c=.o)
CFLAGS = -m32 -ffreestanding -fno-pie -nostdlib -O0
CFLAGS += -Wall -Wextra -Wno-builtin-declaration-mismatch

all : compile
	
compile :
	mkdir -p Binaries
	$(MAKE) $(OBJS)
	$(MAKE) programs
	$(MAKE) link
	$(MAKE) k_binary
	$(ASM) -f bin bootloader/boot.asm -o Binaries/bootloader.bin
	@echo kernel size : $$(wc -c < Binaries/kernel.bin) Octets
	@echo $$(( ($$(wc -c < Binaries/kernel.bin) + 511) / 512 )) secteurs
	secteurs=$$(( ($$(wc -c < Binaries/kernel.bin) + 511) / 512 )); \
	truncate -s $$(( $$secteurs * 512 )) Binaries/kernel.bin; \
	cat Binaries/bootloader.bin Binaries/kernel.bin > Binaries/os.bin; \
	printf "$$(printf '%02x' $$secteurs)" | xxd -r -p | dd of=Binaries/os.bin bs=1 seek=167 conv=notrunc
	$(MAKE) run

programs :
	nasm -f bin Programs/program.asm -o disk.bin

link :
	ld -m elf_i386 -T linker.ld -o Binaries/kernel.elf $(OBJS)

k_binary :
	objcopy -O binary Binaries/kernel.elf Binaries/kernel.bin

run :
	qemu-system-x86_64 -enable-kvm -cpu host -m 2M -monitor stdio -drive format=raw,file=Binaries/os.bin -drive format=raw,file=disk.bin,if=ide

%o : %c
	$(CC) $(CFLAGS) -c $< -o $@

clean :
	rm -f $(shell find . -name "*.o")

fclean : clean
	rm -f Binaries/*



