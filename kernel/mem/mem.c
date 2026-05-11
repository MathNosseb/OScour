#include "mem.h"

uint32_t actual_adr = HEAP_START;

struct heap
{
    int size;//taille en octet de la zone
    uint32_t start;//adresse de debut
    uint32_t end;//adresse de fin
};

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

    uint32_t *content = (uint32_t *)((uint8_t *)bloc + sizeof(struct heap));
    actual_adr = (uint32_t)content + size;
    
    return content;
}