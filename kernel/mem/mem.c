#include "mem.h"
#include "../output/vga.h"
#include "../sys/string.h"
#include "../sys/hex.h"

uint32_t actual_adr = HEAP_START;

struct heap
{
    int size;//taille en octet de la zone
    uint32_t start;//adresse de debut
    uint32_t end;//adresse de fin
    int free;//0 -> libre, 1 -> occupé
    uint32_t precedent;
    
    uint32_t suivant;
    int flag;//flag si suivant est present
};

struct heap *precend_heap;


uint32_t verify_adr_from_base(uint32_t base)
{
    //on verifie si un espace est mis comme libre
    //depuis l origine
    struct heap *header = (struct heap *)base;

    while (header->free != 0 && header->suivant != 0)//tant que l espace n est pas libre
    {
        base += (uint32_t)header->size + (uint32_t)sizeof(struct heap);
        header = (struct heap *)base;
    }
    //on est a l espace libre
    //si le prochain est null alors ca sert a rien de continuer
    //et si il est occupé
    if (header->suivant == 0 && header->free == 1) return 0;

    return base;

}

uint32_t get_stack_ram_usage()
{
    uint32_t esp;
    asm volatile ("mov %%esp, %0" : "=r"(esp));
    return 0x80000 - esp;
}


uint32_t detect_free_mem_space()
{
    //retourne une adresse memoire si elle est libre sinon rien
    uint32_t adr = HEAP_START;
    uint32_t next_adr = verify_adr_from_base(adr);
    while (next_adr != adr)
    {
        
        adr = next_adr;
        next_adr = verify_adr_from_base(adr);
    }
    return next_adr;   
}

int get_heap_ram_usage()
{
    int used = 0;
    struct heap *header = (struct heap *)HEAP_START;
    while (header->flag != 0)
    {  
        if (header->free == 1) used += header->size;
        header = (struct heap *)((uint8_t *)header + sizeof(struct heap) + header->size);
    }
    if (header->free == 1) used += header->size;
    return used;
    
}

void free(void *adr)
{
    struct heap *bloc = (struct heap *)(adr - sizeof(struct heap));

    bloc->free = 0;
}



/*

La fonction d'allocation fonctionne comme ca:
on peut alouer pour un int
int *p = allocate(sizeof(int));

p lui vit dans la stack
ex a 0x200
il retroune en adresse libre 0x100000
p -> [adr: 0x200 | value: 0x100000]
                        ||
                        ||
                        \/
                [adr: 0x100000 | value: ]
on peut assigner sa valeur
*p = 42

*/

/// @brief alloue une quantité de ram dans la heap
/// @param size la quantité en octets a allouer
/// @return retourne un pointeur de l adresse qui est aloué
void *allocate(int size)
{
    struct heap *bloc = (struct heap *)actual_adr;
    bloc->start = actual_adr;
    bloc->end = bloc->start + size;
    bloc->size = size;
    bloc->free = 1;
    bloc->flag = 0;

    uint32_t *content = (uint32_t *)((uint8_t *)bloc + sizeof(struct heap));
    
    if (actual_adr != HEAP_START)
    {
        precend_heap->suivant = (uint32_t)bloc;
        precend_heap->flag = 1;
        bloc->precedent = (uint32_t)precend_heap;
    }
    
    
    
    precend_heap = (struct heap *)actual_adr;

    actual_adr = (uint32_t)content + size;
    
    return content;
}