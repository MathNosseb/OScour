#include "mem.h"

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

/// @brief regroupe la memoire libéré en un seul bloc
void    mem_coalition(void)
{
    struct heap	*h;
    struct heap	*absorb;
    struct heap	*next;
    uint32_t	new_end;

    h = (struct heap*)heap_start;
    while(h)
    {
        
        if (h->canarie != 0xFFFFFFFF)
            KERNEL_PANIC("Erreur Canarie dans coalition memoire");
        //h et le suivant sont libres, on fusionne
        if (!h->free && h->suivant && !h->suivant->free)
        {
            absorb = h->suivant;
            next = absorb->suivant;
            if (next)
                new_end = (uint32_t)next;
            else
                new_end = absorb->end;
            h->suivant = next;
            h->end = new_end;
            h->size = new_end - (uint32_t)h - sizeof(struct heap);
            if (next)
            {
                next->precedent = h;
                h->flag = 1;
            }
            else
            {
                h->flag = 0;
                precend_heap = h;
                actual_adr = h->end;
            }
            continue;//on reteste si il y a encore un bloc vide apres
        }
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
    emplacement = kernel_size + KERNEL_EMPLACEMENT;
    stack_size = STACK_EMPLACEMENT - emplacement;
    itoa(nbr_secteur,secteurs);
    itoa(stack_size, stack);
    kprint_color(secteurs, CYAN_ON_BLACK); kprint_color(" / 54(max) secteurs, ", CYAN_ON_BLACK); 
    kprint_color(stack, CYAN_ON_BLACK); kprint_color(" Octets de stack dispo", CYAN_ON_BLACK);
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
        char number[12]; itoa(index, number);
        char flag[12]; itoa(h->flag, flag);
        char size[12]; itoa(h->size, size);
        char prec[20]; int_to_hex((uint32_t)h->precedent, prec);
        char suiv[20]; int_to_hex((uint32_t)h->suivant, suiv);
        char adrr[20]; int_to_hex((uint32_t)h, adrr);
        char val_adr[20]; int_to_hex((uint32_t)((uint32_t)h+sizeof(struct heap)), val_adr);
        
        kprint("bloc(");kprint(number); kprint(") flag:"); kprint(flag);
        kprint(" size:"); kprint(size);
        kprint_color(h->free == 0 ? " LIBRE" : " OCCUPE", h->free == 0 ? GREEN_ON_BLACK : RED_ON_BLACK);
        kprint(" prec:");kprint(prec);
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
