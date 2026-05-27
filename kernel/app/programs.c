#include "programs.h"
#include "../sys/struct.h"
#include "../output/vga.h"
#include "../sys/string.h"
#include "../sys/hex.h"
#include "../mem/mem.h"


void read_program(void *adr, int size)
{
    //charge le programme dans la ram
    char adresse_char[11]; int_to_char((uint32_t)adr, adresse_char); 

    uint32_t adresse = (uint32_t)0x1000 + ((uint32_t)hex_to_int(adresse_char) - (uint32_t)0x200);
    //lit le program a cet emplacement dans la ram
    for (int i = 0; i < size; i++)
    {
        uint8_t val = *(uint8_t*)adresse;
        char val2[16];
        int_to_hex(val, val2);
        vga_putchar(val2);
        vga_putchar(" ");
        
        adresse+=1;
    }    

}