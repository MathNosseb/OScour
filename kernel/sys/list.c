#include "list.h"

struct Node *create_liste()
{
    //retourne l adresse de la premiere
    struct Node *adressePremierNoeud = allocate(sizeof(struct Node));
    return adressePremierNoeud;
}

struct Node *list_append(int x, struct Node *list)
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
    tete->value = x;

    return tete;
    
}

void afficher_liste(struct Node *list){
    //parcourir la liste jusqu'a la fin
    struct Node *tete;
    tete = list;
    while (tete->addrNext != 0)
    {
        char value_text[sizeof(int)];
        int_to_char(tete->value, value_text);
        vga_putchar(value_text); vga_putchar("\n");
        
        char tete_text[sizeof(struct Node)];
        int_to_char((uint32_t)tete, tete_text);
        vga_putchar(tete_text); vga_putchar("\n");

        char addrNext_text[sizeof(struct Node)];
        int_to_char((uint32_t)tete->addrNext, addrNext_text);
        vga_putchar(addrNext_text); vga_putchar("\n");

        char addrPrecedent_text[sizeof(struct Node)];
        int_to_char((uint32_t)tete->addrPrecedent, addrPrecedent_text);
        vga_putchar(addrPrecedent_text); vga_putchar("\n");
        tete = tete->addrNext;
    }
    
}

void pop_liste(struct Node *list, int index)
{
    if (list == 0) return;
    struct Node *tete;
    tete = list;
    for (int i = 0; i < index; i++)
    {
        if (tete->addrNext != 0)
        {
            tete = tete->addrNext;
        }else
        {
            vga_putchar("ERREUR TAILLE DE LISTE\n");
        }
    }

    char value_text[sizeof(int)];
    int_to_char(tete->value, value_text);
    vga_putchar("supression de l element ");
    vga_putchar(value_text); vga_putchar("\n");

    //si on est en debut de liste
    if (tete->addrPrecedent == 0)
    {
        if (tete->addrNext != 0)
        {
            list = tete->addrNext;
        }

        free(tete);
        
    }
    
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
        free(teteDebut);

        teteDebut = precedent;
    }

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