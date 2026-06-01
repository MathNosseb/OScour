#include "programs.h"
#include "../sys/struct.h"
#include "../output/vga.h"
#include "../sys/string.h"
#include "../sys/hex.h"         
#include "../mem/mem.h"


void load_program(void *adr, int size)
{
    //le programme a deux boucles pour permettre une separation lors de l allocution de la ram
    //entre la heap et la stack
    

    //charge le programme dans la ram
    char adresse_char[11]; int_to_char((uint32_t)adr, adresse_char); 

    //adresse dans la ram du programme
    uint32_t adresse = (uint32_t)0x1000 + ((uint32_t)hex_to_int(adresse_char) - (uint32_t)0x200);

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
    prg_heap-=size;
    vga_putchar("heap:");
    //on avance et on li
    for (int j = 0; j < size; j++)
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
    int_to_char((uint32_t)adr, pointeur);
    

    uint8_t *ptr = (uint8_t*)char_to_int(pointeur);
    uint8_t *end = ptr + size;


    vga_putchar("\nrun program...\n");
    
    while (ptr < end)
    {
        uint8_t val = (uint8_t)*ptr;
        
        if (val == 0x01)
        {
            ptr++;
            while (ptr < end)
            {
                val = (uint8_t)*ptr;
                if (val == 0xFF)
                {
                    ptr++;
                    if ((uint8_t)*ptr == 0xFF) { ptr++; break; }
                }
                char c[2];
                hex_to_string(val, c);
                vga_putchar(c);
                ptr++;
            }
            vga_putchar("\n");
        }
        else if (val == 0x02 || val == 0x03 || val == 0x04 || val == 0x05)
        {
            ptr++;
            
            
            uint8_t a = 0;
            for (int i = 0; i < 4; i++)
            {
                //on recupere chaque partie ex 9C, FF, FF, FF -> 9CFFFFFF
                uint8_t op = (uint8_t)*ptr;
                a = ((uint8_t)a << 8) | op;
                ptr++;
            }

            uint8_t b = 0;
            for (int i = 0; i < 4; i++)
            {
                //on recupere chaque partie ex 9C, FF, FF, FF -> 9CFFFFFF
                uint8_t op = (uint8_t)*ptr;
                b = ((uint8_t)b << 8) | op;
                ptr++;
            }

            uint8_t res = 0;
            if (val == 0x2) res = a + b;
            if (val == 0x3) res = a - b;
            if (val == 0x4) res = a * b;
            if (val == 0x5) res = a / b;

            char text[11];
            int_to_char(res, text);
            vga_putchar(text);
        }
        else if(val == 0x06)
        {
            vga_putchar("mov\n");
            /*mov a b
            premier octet de l operande:
            si on a 0x01 au debut c'est qu on a creation d une variable
            si c est 0x02 c est que la variable existe deja
            seconde octet de l operande:
            si c est 0x01 c est que la deuxieme valeur existe deja
            si c est 0x02 c est que la deuxieme valeur est generique

            les prochains bytes sont:
            [1B longueur][N bytes nom]

            et si le deuxieme est une valeur qui existe deja alors
            [1B longueur][N bytes nom]
            sinon 
            [1B longueur][Data]

            on recupère les 4 premiers octets*/

            //recup du premier octet
            ptr++;//on saute l octet d opcode
            uint8_t create_a = (uint8_t)*ptr;
            ptr++;
            uint8_t create_b = (uint8_t)*ptr;
            ptr++;
            uint8_t len_a = (uint8_t)*ptr;            

            ptr++;
            vga_putchar("end");
            
        }
        else
        {
            ptr++;
        }
    }
}