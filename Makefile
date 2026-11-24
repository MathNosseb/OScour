CC = gcc
ASM = nasm
SOURCES = $(shell find . -name "*.c")
ASM_SOURCES = $(shell find kernel/ -name "*.asm") 
OBJS = $(addprefix Binaries/, $(notdir $(SOURCES:.c=.o)))
ASM_OBJS = $(addprefix Binaries/, $(notdir $(ASM_SOURCES:.asm=.o)))
CFLAGS = -m32 -ffreestanding -fno-pie -nostdlib -O0
CFLAGS += -Wall -Wextra -Wno-builtin-declaration-mismatch
RAM = 4M
vpath %.c $(sort $(dir $(SOURCES)))
vpath %.asm $(sort $(dir $(ASM_SOURCES)))

all : compile
	
compile :
	$(MAKE) $(OBJS)
	$(MAKE) $(ASM_OBJS)
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
	ld -m elf_i386 -T linker.ld -o Binaries/kernel.elf $(OBJS) $(ASM_OBJS)

k_binary :
	objcopy -O binary Binaries/kernel.elf Binaries/kernel.bin

run :
	qemu-system-x86_64 -enable-kvm -cpu host -m $(RAM) -monitor stdio -drive format=raw,file=Binaries/os.bin -drive format=raw,file=disk.bin,if=ide

Binaries/%.o : %.asm | Binaries
	$(ASM) -f elf32 $< -o $@

Binaries/%.o : %.c | Binaries
	$(CC) $(CFLAGS) -c $< -o $@

Binaries :
	mkdir -p Binaries

clean :
	rm -f $(shell find . -name "*.o")

fclean : clean
	rm -f Binaries/*

re : fclean all
