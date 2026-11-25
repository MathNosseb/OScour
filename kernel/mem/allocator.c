#include "allocator.h"

static struct heap	*find_free_bloc(int size)
{
	struct heap	*finder;
	int			find;

	find = 0;
	//verifie qu au moins un bloc a été créé sur la heap
	if (actual_adr != heap_start)
		finder = (struct heap *)heap_start;
	else
		finder = NULL;
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
	if (find)
		return (finder);
	return (NULL);
}

static uint32_t	*split_bloc(struct heap *finder, int size)
{
	uint32_t	*content;
	struct heap	*new_finder;

	//on change pas le suivant ni le precendent mais on créer un espace vide (orphelin)
	finder->end = (uint32_t)finder + size + sizeof(struct heap);
	finder->size = size;
	finder->free = 1;
	if (finder->suivant != 0)
		finder->flag = 1;
	content = (uint32_t *)((uint8_t *)finder + sizeof(struct heap));
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
		new_finder = (struct heap *)finder->end;
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
	return (content);
}

static uint32_t	*create_bloc(int size)
{
	struct heap	*bloc;
	uint32_t	*content;
	
	bloc = (struct heap *)actual_adr;
	bloc->suivant = NULL;
	bloc->precedent = NULL;
	bloc->canarie = 0xFFFFFFFF;
	bloc->end = (uint32_t)bloc + size + sizeof(struct heap);
	bloc->size = size;
	bloc->free = 1;
	bloc->flag = 0;
	content = (uint32_t *)((uint8_t *)bloc + sizeof(struct heap));
	if (actual_adr != heap_start)
	{
	    precend_heap->suivant = bloc;
	    precend_heap->flag = 1;
	    bloc->precedent = precend_heap;
	}
	precend_heap = (struct heap *)actual_adr;
	actual_adr = (uint32_t)content + size;
	return (content);
}

/// @brief alloue une quantité de ram dans la heap
/// @param size la quantité en octets a allouer
/// @return retourne un pointeur de l adresse qui est aloué
void    *allocate(int size)
{
	//on va essayer de trouver un bloc libre qui a une taille bien libre
	struct heap *finder;

	if (size < 0)
	    KERNEL_PANIC("ALLOCATION trop petite");
	finder = find_free_bloc(size);
	if (finder)//si un bloc libre est trouvé
	    return (split_bloc(finder, size));
	if ((actual_adr - heap_start) + sizeof(struct heap) + size > heap_max)
	    KERNEL_PANIC("Memoire pleine");
	return (create_bloc(size));
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