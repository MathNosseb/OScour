#include "mem_info.h"

/// @brief retourne la quantité de stack utilisé
uint32_t    get_stack_usage(void)
{
    uint32_t	esp;

    asm volatile ("mov %%esp, %0" : "=r"(esp));
    return (STACK_EMPLACEMENT - esp);
}

/// @brief retourne la quantité de ram dans la heap utilisé
int	get_heap_ram_usage(void)
{
    struct heap	*header;
    int			used;

	used = 0;
    if (actual_adr != heap_start)
        header = (struct heap *)heap_start;
    else
        header = 0;
    while (header && header->flag != 0)
    {  
        if (header->free == 1) used += header->size;
        header = header->suivant;
    }
    if (header->free == 1) used += header->size;
    return used;
    
}

/// @brief la quantité TOTAL de ram utilisé par l OS
uint32_t	get_mem_usage(void)
{
    uint32_t	mem;
    uint32_t	kernel_size;
    uint32_t	emplacement;

	uint32_t	stack_size;
    int			nbr_secteur;
    
    mem = get_total_ram_size();
    nbr_secteur = *(int *)SECTORS_NUMBER;
    kernel_size = nbr_secteur * 512;
    emplacement = kernel_size + 0x1000;
    stack_size = STACK_EMPLACEMENT - emplacement;
    mem -= stack_size;
    mem -= get_heap_size();
    mem += get_stack_usage();
    mem += actual_adr - heap_start;
    return (mem);
}

/// @brief la quantité de ram parcouru par l allocateur
uint32_t get_heap_ram_usage_and_non_use()
{
    struct heap *header;

    if (actual_adr != heap_start)
        header = (struct heap *)heap_start;
    else
        header = 0;
    while (header->flag != 0)
    {  
        header = header->suivant;
    }
    uint32_t size = (uint32_t)((uint32_t)header+sizeof(struct heap));
    return size - heap_start;
    
}