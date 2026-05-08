#include "vga.h"

int counter = 0;

/// @brief affiche une suite de caracteres a l ecran
/// @param texte le texte a afficher sous forme de string ""
/// @param counter l adresse du conteur de caractere
void vga_putchar(char *texte)
{
    
    vga_puchar_color(texte, WHITE_ON_BLACK);
    
}

void vga_puchar_color(char *texte, uint8_t color)
{
    volatile uint16_t* p;
    p = (volatile uint16_t *)VIDEO_MEMORY;

    int index = 0;
    while (texte[index] != '\0')
    {
        int special = 0;
        if (texte[index] == '\n')
        {
           special = 1;
            counter = (counter / 80 + 1) * 80;  // pas de -= 1
            index++;
            
        }
        if (texte[index] == '\b')
        {
            //backspace
            special=1;
            counter-=1;
            index+=1;
            vga_putchar(" ");
            counter-=1;
            continue;
        }
        if (texte[index] == '\t')
        {
            //tabulation
            special=1;
            counter += 3;
            index++;
            break;
            
        }
        if (special==0)
        {
            p[counter] = (color << 8) | texte[index];
            counter+=1;
            index+=1;
        }
        
    }
}

void update_cursor()
{
    counter = (counter < 0) ? 0 : counter % (25*80);
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(counter & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)(counter >> 8));
}

void set_cursor(int position)
{
    counter = position;
    update_cursor();
}

int get_cursor()
{
    return counter;
}

void clear_screen()
{
    for (int i = 0; i < 25*80; i++)
    {
        vga_putchar(" ");
        update_cursor();
    }

    set_cursor(0);
    
}

