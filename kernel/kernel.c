#include "output/vga.h"
#include "mem/mem.h"
#include "app/shell.h"
#include "app/banner.h"
#include "disque/disk.h"
#include "sys/interuptions/interuption.h"

#define FUNCTIONS_TABLE 0x504
#define SECTORS_NUMBER 0x500

/// @brief met les adresses des fonctions systèmes en mémoire
static void	put_fonctions(void)
{
	void	**table;

	table = (void **)FUNCTIONS_TABLE;
	*table = (void *)kprint;
}

__attribute__((section(".text.entry")))
void	_start(void)
{
	load_art();
	init_entry();
	write_ata(0, 0, (uint8_t[]){0xE8, 0, 0, 0, 0, 0x5B, 0x81, 0xC3, 0x1B, 0, 0, 0, 0x83, 0xEB, 5, 0x53, 0xA1, 4, 5, 0, 0, 0xFF, 0xD0, 0x83, 0xC4, 4, 0xC3, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64}, 38);
	init_memory_info();
	put_fonctions();
	shell_loop();
}
