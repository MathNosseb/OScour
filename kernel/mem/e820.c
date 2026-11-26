#include "mem.h"
#include "e820.h"

/// @brief donne la taille total de la ram dans l'OS
uint64_t	get_total_ram_size(void)
{
	struct e820_entry	*entries;
	uint16_t			count;
	uint64_t			total;

	total = 0;
	count = *(uint16_t *)ENTRIE_COUNT;
	entries = (struct e820_entry *)ENTRIE_START;
	while (count)
	{
		total += entries[count - 1].length;
		count--;
	}
	return (total);
}

/// @brief affiche les entrées mémoire et set le debut de la heap
/// @brief choisis l entrées la plus grosse
void	set_e820_entries(void)
{
	struct e820_entry	*entries;
	struct e820_entry	biggest;
	uint16_t			count;
	uint32_t			tot;
	uint32_t			i;
	char				base[16];
	
	entries = (struct e820_entry *)ENTRIE_START;
	count = *(uint16_t *)ENTRIE_COUNT;
	biggest.type = 2;
	biggest.length = 0;
	tot = 0;
	i = 0;
	while (i < count)
	{
		if (entries[i].base != 0 && entries[i].type == 1 &&
			entries[i].length > biggest.length)
			biggest = entries[i];
		int_to_hex(entries[i].base, base);
		kprint("0x");
		kprint(base); kprint (" ");
		put_size(entries[i].length);
		//kput_number(entries[i].length/1024);
		kprint(" ");
		kput_number(entries[i].type);
		
		if (entries[i].type == 1 && i != 0)
			kprint_color(" usable\n", GREEN_ON_BLACK);
		else
			kprint_color(" reserved\n", RED_ON_BLACK);\
		
		tot += entries[i++].length;
	}
	if (biggest.type == 2)
	    KERNEL_PANIC("pas assez de ram");
	else
	{
	    actual_adr = biggest.base;
	    heap_start = biggest.base;
	    heap_max = biggest.length;
	    kput_number(tot/1024);
	    kprint("Ko de ram total\n");
	}
}

/// @brief retourne la taille max de la heap
uint32_t	get_heap_size(void)
{
	struct e820_entry	*entries;
	struct e820_entry	heap;
	uint16_t			count;
	int					i;
	
	entries = (struct e820_entry *)ENTRIE_START;
	count = *(uint16_t *)ENTRIE_COUNT;
	heap.length = 0;
	i = 0;
	while (i < count)
	{
		if (entries[i].base != 0 && entries[i].type == 1 &&
			entries[i].length > heap.length)
			heap = entries[i];
		i++;
	}
	return (heap.length);
}