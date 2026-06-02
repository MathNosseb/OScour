#include "mem.h"
#include "../output/vga.h"
#include "../sys/string.h"
#include "../sys/hex.h"

uint32_t actual_adr = HEAP_START;



struct e820_entry
{
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t acpi;
} __attribute__((packed));

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

uint32_t get_heap_ram_usage_and_non_use()
{
    struct heap *header = (struct heap *)HEAP_START;
    while (header->flag != 0)
    {  
        header = (struct heap *)header->suivant;
    }
    uint32_t size = (uint32_t)((uint32_t)header+sizeof(struct heap));
    return size - HEAP_START;
    
}

void free(void *adr)
{
    struct heap *h = (struct heap *)((uint32_t)adr - sizeof(struct heap));
    h->free = 0;
}

uint64_t get_total_ram()
{
    struct e820_entry *entries = (struct e820_entry *)0x8004;
    uint16_t count = *(uint16_t *)0x8000;

    uint64_t total = 0;

    for (int i = 0; i < count; i++)
    {
        if (entries[i].type == 1)
        {
            total += entries[i].length;
        }
    }

    return total;
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
        char prec[20]; int_to_hex(h->precedent, prec);
        char suiv[20]; int_to_hex(h->suivant, suiv);
        char adrr[20]; int_to_hex((uint32_t)h, adrr);
        char val_adr[20]; int_to_hex((uint32_t)((uint32_t)h+sizeof(struct heap)), val_adr);
        
        vga_putchar("bloc(");vga_putchar(number); vga_putchar(") flag:"); vga_putchar(flag);
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

void heap_stress_test()
{
    vga_putchar("\nstress");
    void *ptrs[200];

    for (int cycle = 0; cycle < 2; cycle++)
    {
        // phase 1 : allocations variées
        for (int i = 0; i < 200; i++)
        {
            int size = (i * cycle + 13) % 64 + 1;
            ptrs[i] = allocate(size);

            if (ptrs[i])
            {
                uint8_t *p = (uint8_t *)ptrs[i];

                // pattern dépendant du contexte (pour détecter corruption)
                for (int j = 0; j < size; j++)
                {
                    p[j] = (uint8_t)(i ^ j ^ cycle);
                }
            }
        }

        // phase 2 : vérification + corruption détectable
        for (int i = 0; i < 200; i++)
        {
            if (!ptrs[i]) continue;

            uint8_t *p = (uint8_t *)ptrs[i];
            int size = (i * cycle + 13) % 64 + 1;

            for (int j = 0; j < size; j++)
            {
                if (p[j] != (uint8_t)(i ^ j ^ cycle))
                {
                    // corruption détectée (tu peux breakpoint ici)
                    *((volatile int *)0xDEAD) = 1;
                    vga_putchar("\ncorruption");
                }
            }
        }

        // phase 3 : libération partielle (fragmentation)
        for (int i = 0; i < 200; i += 2)
        {
            free(ptrs[i]);
            ptrs[i] = 0;
        }

        // phase 4 : re-allocation pour forcer réutilisation de trous
        for (int i = 0; i < 100; i++)
        {
            ptrs[i] = allocate((i % 32) + 1);
        }
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
        finder->end = finder->start + size + sizeof(struct heap);
        finder->size = size;
        finder->free = 1;
        if (finder->suivant != 0) finder->flag = 1;
        uint32_t *content = (uint32_t *)((uint8_t *)finder + sizeof(struct heap));

        //determiner si on peut creer un sous bloc pour eviter de creer un espace orphelin
        //espace minimal requis -> taille header + 1 octet
        if (finder->flag == 0) return content;
        if (((struct heap *)finder->suivant)->start - finder->end >= sizeof(struct heap) + 1)
        {
            //on créer un espace
            struct heap *new_finder = (struct heap *)finder->end;
            new_finder->start = finder->end;
            new_finder->end = ((struct heap *)finder->suivant)->start;
            new_finder->free = 0;
            new_finder->flag = 1;
            new_finder->size = ((struct heap *)finder->suivant)->start - finder->end - sizeof(struct heap);
            new_finder->suivant = ((struct heap *)finder->suivant)->start;
            new_finder->precedent = finder->start;
            
            //redimensionner le precedent
            finder->suivant = new_finder->start;

            //redimensionner le precedent
            ((struct heap *)new_finder->suivant)->precedent = new_finder->start;
            
        }

        return content;
    }

    
    struct heap *bloc = (struct heap *)actual_adr;
    bloc->start = actual_adr;
    bloc->end = bloc->start + size + sizeof(struct heap);
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

