#include "mem.h"
#include "../output/vga.h"
#include "../sys/string.h"
#include "../sys/hex.h"

struct heap *precend_heap = 0;
uint32_t    actual_adr = 0;
uint32_t    heap_start = 0;
uint32_t    heap_max = 0;

void    KERNEL_PANIC(const char *s)
{
    kernel_panic_art();
    kprint_color("KERNEL PANIC", RED_ON_BLACK);
    kprint_color(s, RED_ON_BLACK);
    kprint("\n");
    while (1)
        __asm__ volatile ("hlt");
}

void    mem_coalition(void)
{
    struct heap *h;
    struct heap *absorb;

    h = (struct heap*)heap_start;
    while(h)
    {
        
        if (h->canarie != 0xFFFFFFFF)
            KERNEL_PANIC("Erreur Canarie dans coalition memoire");
        if (!h->free && h->suivant && !h->suivant->free)
        {
            h->size = h->suivant->end - (uint32_t)h - sizeof(struct heap);
            absorb = h->suivant;
            h->suivant = h->suivant->suivant;
            if (h->suivant)
            {
                h->suivant->precedent = h;
                //laisser comme ca pour absorber les espaces mort
                //ne pas utiliser la size presumé
                h->end = (uint32_t)h->suivant;
                h->size = (uint32_t)absorb + absorb->size - (uint32_t)h - sizeof(struct heap);
                h->flag = 1;
            }
            else
            {
                precend_heap = h;
                h->end = (uint32_t)h + h->size + sizeof(struct heap);
                h->suivant = NULL;
                h->flag = 0;
            }
        }
        //autre boucle avec le nouveau suivant
        //ne pas l inclure dans la boucle precendente
        if (!h->free && h->suivant && !h->suivant->free)
        {
            //on reste la ou on est car on va encore reduire
        }
        else
            h = h->suivant;
    }
}

/// @brief premiere fonction appellé de gestion mémoire
/// @brief affiche le nombre de secteurs chargés
/// @brief affiche la stack disponible
void	init_memory_info(void)
{
	char		secteurs[16];
	char		stack[16];
	int			nbr_secteur;
	uint32_t	kernel_size;
	uint32_t	emplacement;
	uint32_t	stack_size;

	set_e820_entries();
	nbr_secteur = *(int *)SECTORS_NUMBER;
    kernel_size = nbr_secteur * 512;
    emplacement = kernel_size + 0x1000;
    stack_size = STACK_EMPLACEMENT - emplacement;
    itoa(nbr_secteur,secteurs);
    itoa(stack_size, stack);
    kprint_color(secteurs, CYAN_ON_BLACK); kprint_color(" secteurs, ", CYAN_ON_BLACK); 
    kprint_color(stack, CYAN_ON_BLACK); kprint_color(" Octets de stack dispo", CYAN_ON_BLACK);
}

uint32_t    get_stack_usage(void)
{
    uint32_t	esp;

    asm volatile ("mov %%esp, %0" : "=r"(esp));
    return (STACK_EMPLACEMENT - esp);
}

int	get_heap_ram_usage(void)
{
    struct heap *header;
    int used = 0;

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
    emplacement = kernel_size + 0x1000;\
    stack_size = STACK_EMPLACEMENT - emplacement;
    mem -= stack_size;
    mem -= get_heap_size();
    mem += get_stack_usage();
    mem += actual_adr - heap_start;
    return (mem);
}

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

void free(void *adr)
{
    if (!adr) return ;
    if ((uint32_t)adr < heap_start + sizeof(struct heap))
        KERNEL_PANIC("FREE hors tas");
    struct heap *h = (struct heap *)((uint32_t)adr - sizeof(struct heap));
    if (h->canarie != 0xFFFFFFFF) KERNEL_PANIC("FREE canarie");
    if (h->free == 0) KERNEL_PANIC("FREE double free");
    h->free = 0;
    mem_coalition();
}

void dump_heap()
{
    struct heap *h;

    kprint("\n");
    if (actual_adr != heap_start)
        h = (struct heap *)heap_start;
    else
        h = 0;
    int index = 0;
    while (h)
    {
        char number[5]; itoa(index, number);
        char flag[5]; itoa(h->flag, flag);
        char size[5]; itoa(h->size, size);
        char free[5]; itoa(h->free, free);
        char prec[20]; int_to_hex((uint32_t)h->precedent, prec);
        char suiv[20]; int_to_hex((uint32_t)h->suivant, suiv);
        char adrr[20]; int_to_hex((uint32_t)h, adrr);
        char val_adr[20]; int_to_hex((uint32_t)((uint32_t)h+sizeof(struct heap)), val_adr);
        
        kprint("bloc(");kprint(number); kprint(") flag:"); kprint(flag);
        kprint(" size:"); kprint(size);
        kprint(" free:");kprint(free); kprint(" prec:");kprint(prec);
        kprint(" suiv:");kprint(suiv);kprint(" head:"); kprint(adrr);
        kprint(" val:"); kprint(val_adr);
        if (h->flag == 0) break;
        kprint("\n");
        h = h->suivant;
        index++;
    }
    
}

void heap_stress_test()
{
    kprint("\nstress");
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
                    kprint("\ncorruption");
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
        kprint("done");
        // phase 5 : libération totale
        for (int i = 0; i < 200; i++)
        {
            free(ptrs[i]);
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
void    *allocate(int size)
{
    //on va essayer de trouver un bloc libre qui a une taille bien libre
    struct heap *finder;
    int find;

    if (size < 0)
        KERNEL_PANIC("ALLOCATION trop petite");

    find = 0;
    if (actual_adr != heap_start)
        finder = (struct heap *)heap_start;
    else
        finder = 0;
    while (finder)
    {
        if (finder->canarie != 0xFFFFFFFF)
            KERNEL_PANIC("finder canarie");
        if (finder->suivant)
        {
            if ((finder->suivant)->canarie != 0xFFFFFFFF)
                KERNEL_PANIC("canarie suivant");
        }
        if (finder->free == 0 && finder->size >= size)
        {
            find = 1;
            break;
        }
        finder = finder->suivant;
    }

    if (find)//si un bloc libre est trouvé
    {
        //on change pas le suivant ni le precendent mais on créer un espace vide (orphelin)
        finder->end = (uint32_t)finder + size + sizeof(struct heap);
        finder->size = size;
        finder->free = 1;
        if (finder->suivant != 0) finder->flag = 1;
        uint32_t *content = (uint32_t *)((uint8_t *)finder + sizeof(struct heap));

        //determiner si on peut creer un sous bloc pour eviter de creer un espace orphelin
        //espace minimal requis -> taille header + 1 octet
        if (finder->flag == 0)
        {
            actual_adr = finder->end;
            return (content);
        }
        if ((uint32_t)(finder->suivant) - finder->end >= sizeof(struct heap) + 1)
        {
            //on créer un espace
            struct heap *new_finder = (struct heap *)finder->end;
            new_finder->canarie = 0xFFFFFFFF;
            new_finder->end = (uint32_t)(finder->suivant);
            new_finder->free = 0;
            new_finder->flag = 1;
            new_finder->size = (uint32_t)(finder->suivant) - finder->end - sizeof(struct heap);
            new_finder->suivant = finder->suivant;
            new_finder->precedent = finder;
            
            //redimensionner le precedent
            finder->suivant = new_finder;

            //redimensionner le precedent
            new_finder->suivant->precedent = new_finder;
            
        }
        return content;
    }
    if ((actual_adr - heap_start) + sizeof(struct heap) + size > heap_max)
        KERNEL_PANIC("Memoire pleine");
    struct heap *bloc = (struct heap *)actual_adr;\
    bloc->suivant = NULL;
    bloc->precedent = NULL;
    bloc->canarie = 0xFFFFFFFF;
    bloc->end = (uint32_t)bloc + size + sizeof(struct heap);
    bloc->size = size;
    bloc->free = 1;
    bloc->flag = 0;

    uint32_t *content = (uint32_t *)((uint8_t *)bloc + sizeof(struct heap));
    
    if (actual_adr != heap_start)
    {
        precend_heap->suivant = bloc;
        precend_heap->flag = 1;
        bloc->precedent = precend_heap;
    }
    
    
    
    precend_heap = (struct heap *)actual_adr;

    actual_adr = (uint32_t)content + size;
    
    return content;
}

