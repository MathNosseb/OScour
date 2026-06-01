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
    int value;
};

struct Node *create_liste();
struct Node *list_append(int, struct Node *);
void afficher_liste(struct Node *);
void clear_liste(struct Node *);
int get_list_size(struct Node *);
void pop_liste(struct Node *, int);