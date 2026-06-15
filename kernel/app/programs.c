#include "programs.h"
#include "../sys/struct.h"
#include "../output/vga.h"
#include "../sys/string.h"
#include "../sys/hex.h"         
#include "../mem/mem.h"
#include "../sys/list.h"


void load_program(void *adr, int size)
{
    //le programme a deux boucles pour permettre une separation lors de l allocution de la ram
    //entre la heap et la stack
    
    //charge le programme dans la ram
    char adresse_char[11]; int_to_char((uint32_t)adr, adresse_char); 

    //adresse dans la ram du programme
    uint32_t adresse = (uint32_t)0x1000 + ((uint32_t)char_to_int(adresse_char) - (uint32_t)0x200);

    //le mettre sur la heap
    //adresse du programme sur la heap
    uint8_t *prg_heap = allocate(size);

    //on affiche l adresse
    char val_adr[11]; 
    int_to_hex((uint32_t)prg_heap, val_adr);
    vga_putchar(val_adr);//adresse que l on pourra free

    vga_putchar("\n");    

    //lit le program a cet emplacement dans la ram
    for (int i = 0; i < size; i++)
    {
        uint8_t val = *(uint8_t*)adresse;//quel est la valeur de l octet

        //on va le mettre sur la heap
        *prg_heap = val;
        
        adresse+=1;//prochaine adresse (caractère)
        prg_heap+=1;
    }    

    //lire dans la ram
    //reset du pointeur
    prg_heap-=(size + sizeof(uint32_t));
    vga_putchar("heap:");
    //on avance et on li
    for (int j = 0; j < size + sizeof(uint32_t); j++)
    {
        uint8_t valeur = *(uint8_t *) prg_heap;
        char valeur_texte[11];
        int_to_hex(valeur, valeur_texte);
        vga_putchar(valeur_texte); vga_putchar (" ");

        prg_heap+=1;
    }
    
}


void run_program(void *adr, int size)
{
    char pointeur[11];
    int_to_char((uint32_t)adr, pointeur); vga_putchar(pointeur);
    

    uint8_t *ptr = (uint8_t*)char_to_int(pointeur);

    vga_putchar("\nrun program...\n");
    void (*func)() = (void (*)())ptr;
    func();
    vga_putchar("\nend program...\n");
}