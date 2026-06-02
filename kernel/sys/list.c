#include "list.h"

struct Node *create_liste()
{
    //retourne l adresse de la premiere
    struct Node *adressePremierNoeud = allocate(sizeof(struct Node));
    adressePremierNoeud->addrNext = 0;
    adressePremierNoeud->addrPrecedent = 0;
    adressePremierNoeud->value = 0;
    return adressePremierNoeud;
}

struct Node *list_append(uint32_t *adr, uint32_t size, struct Node *list)
{
    //avancer jusqu'au flag de fin
    struct Node *tete;
    tete = list;
    while (tete->addrNext != 0)
    {
        tete = tete->addrNext; 
    }
    //une fois que on arrive au flag
    //on ajoute une adresse

    tete->addrNext = allocate(sizeof(struct Node));
    tete->addrNext->addrPrecedent = tete;
    tete->addrNext->addrNext = 0;
    tete->addrNext->value = 0;
    tete->value = adr;
    tete->size = size;

    return tete->addrNext;
    
}

void afficher_liste(struct Node *list){
    //parcourir la liste jusqu'a la fin
    //ne fonctionne que pour les int
    struct Node *tete;
    tete = list;
    while (tete->addrNext != 0)
    {
        char value_text[sizeof(tete->value)];
        int_to_char(*(uint32_t *)tete->value, value_text);
        vga_putchar(value_text); vga_putchar("\n");
        /*
        char tete_text[sizeof(struct Node)];
        int_to_char((uint32_t)tete, tete_text);
        vga_putchar(tete_text); vga_putchar("\n");

        char addrNext_text[sizeof(struct Node)];
        int_to_char((uint32_t)tete->addrNext, addrNext_text);
        vga_putchar(addrNext_text); vga_putchar("\n");

        char addrPrecedent_text[sizeof(struct Node)];
        int_to_char((uint32_t)tete->addrPrecedent, addrPrecedent_text);
        vga_putchar(addrPrecedent_text); vga_putchar("\n");*/
        tete = tete->addrNext;
    }
    
}

struct Node *pop_liste(struct Node *list, int index)
{
    if (list == 0) return 0;

    struct Node *current = list;

    for (int i = 0; i < index; i++)
    {
        if (current->addrNext == 0)
            return list; // index hors limite

        current = current->addrNext;
    }

    // debug optionnel
    char value_text[16];
    int_to_char((uint32_t)current->value, value_text);
    vga_putchar("suppression: ");
    vga_putchar(value_text);
    vga_putchar("\n");

    if (current->addrPrecedent != 0)
        current->addrPrecedent->addrNext = current->addrNext;
    else
        list = current->addrNext; // nouvelle tête

    // si on n'est pas en fin
    if (current->addrNext != 0)
        current->addrNext->addrPrecedent = current->addrPrecedent;
    free(current->value);
    free(current);

    return list;
}

void clear_liste(struct Node *list){
    if (list == 0) return;
    //parcourir la liste jusqu'a la fin
    struct Node *tete;
    tete = list;
    while (tete->addrNext != 0)
    {
        tete = tete->addrNext;
    }
    //on est tout a la fin
    struct Node *teteDebut;
    teteDebut = tete;
    struct Node *precedent = teteDebut->addrPrecedent;
    while(precedent != 0){
        //on save l adresse de l objet precedent pour y revenir
        precedent = teteDebut->addrPrecedent;
        free(teteDebut->value);
        free(teteDebut);

        teteDebut = precedent;
    }
    free(teteDebut->value);
    free(teteDebut);

}


int get_list_size(struct Node *list)
{
    int conteur = 0;
    //avancer jusqu'au flag de fin
    struct Node *tete;
    tete = list;
    while (tete->addrNext != 0)
    {
        tete = tete->addrNext; 
        conteur++;
    }
    return conteur;
}