#include "output/vga.h"
#include "mem/mem.h"
#include "app/shell.h"
#include "app/banner.h"

#define FUNCTIONS_TABLE 0x504
#define SECTORS_NUMBER 0x500

/// @brief met les adresses des fonctions systèmes en mémoire
static void put_fonctions(void)
{
	void    **table;

	table = (void **)FUNCTIONS_TABLE;
	*table = (void *)kprint;
}

__attribute__((section(".text.entry")))
void	_start(void)
{
    load_art();
    init_memory_info();
    put_fonctions();
    shell_loop();
}
