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


uint32_t get_stack_ram_usage()
{
    uint32_t esp;
    asm volatile ("mov %%esp, %0" : "=r"(esp));
    return 0x80000 - esp;
}

int get_heap_ram_usage()
{
    int used = 0;
    struct heap *header = (struct heap *)HEAP_START;
    while (header->flag != 0)
    {  
        if (header->free == 1) used += header->size;
        header = (struct heap *)header->suivant;
    }
    if (header->free == 1) used += header->size;
    return used;
    
}

void free(void *adr)
{
    struct heap *h = (struct heap *)((uint32_t)adr - sizeof(struct heap));
    h->free = 0;
}

void dump_heap()
{
    vga_putchar("\n");
    struct heap *h = (struct heap *)HEAP_START;
    int index = 0;
    while (1)
    {
        char number[2]; int_to_char(index, number);
        char flag[5]; int_to_char(h->flag, flag);
        char size[5]; int_to_char(h->size, size);
        char free[5]; int_to_char(h->free, free);
        char prec[20]; int_to_char(h->precedent, prec);
        char suiv[20]; int_to_char(h->suivant, suiv);
        char adrr[20]; int_to_char((uint32_t)h, adrr);
        char val_adr[20]; int_to_char((uint32_t)((uint32_t)h+sizeof(struct heap)), val_adr);
        
        vga_putchar(" bloc(");vga_putchar(number); vga_putchar(") flag:"); vga_putchar(flag);
         vga_putchar(" size:"); vga_putchar(size);
        vga_putchar(" free:");vga_putchar(free); vga_putchar(" prec:");vga_putchar(prec);
        vga_putchar(" suiv:");vga_putchar(suiv);vga_putchar(" adr:"); vga_putchar(adrr);
        vga_putchar(" val:"); vga_putchar(val_adr);
        vga_putchar("\n");
        if (h->flag == 0) break;
        h = (struct heap *)h->suivant;
        index++;
    }
    
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
    //on va essayer de trouver un bloc libre qui a une taille bien libre
    struct heap *finder = (struct heap *)HEAP_START;//on part de l origine
    int find = 0;
    while (1)
    {
        if (finder->free == 0 && finder->size >= size)
        {
            find = 1;
            break;
        }
        if (finder->flag == 0) break;
        finder = (struct heap *)finder->suivant;
    }

    
    if (find)//si un bloc libre est trouvé
    {
        uint32_t use_adresse = (uint32_t)finder;//adresse du bloc
        //on change pas le suivant ni le precendent mais on créer un espace vide (orphelin)
        finder->end = finder->start + size;
        finder->size = size;
        finder->free = 1;
        if (finder->suivant != 0) finder->flag = 1;
        uint32_t *content = (uint32_t *)((uint8_t *)finder + sizeof(struct heap));
        return content;
    }

    
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