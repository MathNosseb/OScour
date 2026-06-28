#include "vga.h"

int counter = 0;

/// @brief affiche une suite de caracteres a l ecran -> doit rester en permier pour avoir la meme adr
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
            update_cursor();
            
        }
        if (texte[index] == '\b')
        {
            //backspace
            special=1;
            counter-=1;
            index+=1;
            vga_putchar(" ");
            counter-=1;
            update_cursor();
            continue;
        }
        if (texte[index] == '\t')
        {
            //tabulation
            special=1;
            counter += 3;
            index++;
            update_cursor();
            break;
            
        }
        if (special==0)
        {
            p[counter] = (color << 8) | texte[index];
            counter+=1;
            index+=1;
            update_cursor();
        }
        
    }
}

void update_cursor()
{
    if (counter >= 80*25)
    {
        counter -= 80;
        scroll_screen();
    }
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
    volatile uint16_t* p;
    p = (volatile uint16_t *)VIDEO_MEMORY;
    for (int i = 0; i < 25*80; i++)
    {
        p[i] = (WHITE_ON_BLACK << 8) | ' ';
    }

    set_cursor(0);
    update_cursor();
}



void load_art()
{
    vga_putchar("   ____   _____                             ___  \n");
    vga_putchar("  / __ \\ / ____|                           |__ \\ \n");
    vga_putchar(" | |  | | (___   ___ ___  _   _ _ __  __   __ ) |\n");
    vga_putchar(" | |  | |\\___ \\ / __/ _ \\| | | | '__| \\ \\ / // / \n");
    vga_putchar(" | |__| |____) | (_| (_) | |_| | |     \\ V // /_ \n");
    vga_putchar("  \\____/|_____/ \\___\\___/ \\__,_|_|      \\_/|____|\n");
    vga_puchar_color("Oscour made by mathnosseb v2.1.0\n", CYAN_ON_BLACK);
}

void print_at(int x, int y, char *text)
{
    volatile uint16_t* p;
    p = (volatile uint16_t *)VIDEO_MEMORY;

    int index = 0;
    while (text[index] != '\0')
    {
        p[y * 80 + x + index] = (GREEN_ON_BLACK << 8) | text[index];
        index++;
    }

    
}

void scroll_screen()
{
    //deplace toutes les lignes vers le haut sauf la première
    //pour toutes les lignes faire -80 a leur position
    
    //ligne = caractere + couleur -> 2 octets
    //chaque ligne = 160 octets

    volatile uint16_t* p;
    p = (volatile uint16_t *)VIDEO_MEMORY;
    //on copie du bas vers le haut

    for (int j = 0; j < 24; j++)
    {
        for (int i = 0; i < 80; i++)
        {
            p[j * 80 + i] = p[(j + 1) * 80 + i];
        }
    }
    //clear la derniere ligne

    for (int i = 0; i < 80; i++)
        p[80*24+i] = (WHITE_ON_BLACK << 8) | ' ';

}