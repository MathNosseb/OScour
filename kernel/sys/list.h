#pragma once
#include "../mem/mem.h"
#include "../output/vga.h"
#include "struct.h"
#include "string.h"
#include "hex.h"

struct Node
{
    struct Node *addrNext;
    struct Node *addrPrecedent;
    void *value;
    uint32_t size;
};

struct Node *create_liste();
struct Node *list_append(uint32_t *adr, uint32_t size, struct Node *list);
void afficher_liste(struct Node *);
void clear_liste(struct Node *);
int get_list_size(struct Node *);
struct Node *pop_liste(struct Node *list, int index);