#include "vga.h"

int g_counter = 0;

/// @brief affiche une suite de caracteres a l ecran -> doit rester en permier pour avoir la meme adr
/// @param texte le texte a afficher sous forme de string ""
/// @param g_counter l adresse du conteur de caractere
void kprint(char *texte)
{
    kprint_color(texte, WHITE_ON_BLACK);
}

void    write_char(char c)
{
    char str[2];

    str[0] = c;
    str[1] = '\0';
    kprint(str);
}

void kprint_color(char *texte, uint8_t color)
{
    volatile uint16_t* p;
    int index = 0;

    p = (volatile uint16_t *)VIDEO_MEMORY;    
    while (texte[index] != '\0')
    {
        int special = 0;
        if (texte[index] == '\n')
        {
           special = 1;
            g_counter = (g_counter / 80 + 1) * 80;  // pas de -= 1
            index++;
            update_cursor();
        }
        if (texte[index] == '\b')
        {
            //backspace
            special=1;
            g_counter-=1;
            index+=1;
            kprint(" ");
            g_counter-=1;
            update_cursor();
            continue;
        }
        if (texte[index] == '\t')
        {
            //tabulation
            special=1;
            g_counter += 3;
            index++;
            update_cursor();
            break;
        }
        if (special==0)
        {
            p[g_counter] = (color << 8) | texte[index];
            g_counter+=1;
            index+=1;
            update_cursor();
        }
    }
}

void update_cursor()
{
    if (g_counter >= 80*25)
    {
        g_counter -= 80;
        scroll_screen();
    }
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(g_counter & 0xFF));

    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)(g_counter >> 8));
}

void set_cursor(int position)
{
    g_counter = position;
    update_cursor();
}

int get_cursor()
{
    return g_counter;
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
    kprint("   ____   _____                             ___  \n");
    kprint("  / __ \\ / ____|                           |__ \\ \n");
    kprint(" | |  | | (___   ___ ___  _   _ _ __  __   __ ) |\n");
    kprint(" | |  | |\\___ \\ / __/ _ \\| | | | '__| \\ \\ / // / \n");
    kprint(" | |__| |____) | (_| (_) | |_| | |     \\ V // /_ \n");
    kprint("  \\____/|_____/ \\___\\___/ \\__,_|_|      \\_/|____|\n");
    kprint_color("Oscour made by mathnosseb v2.1.0\n", CYAN_ON_BLACK);
}

void print_at(int x, int y, char *text)
{
    volatile uint16_t* p;
    int index = 0;

    p = (volatile uint16_t *)VIDEO_MEMORY;
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
            p[j * 80 + i] = p[(j + 1) * 80 + i];
    }
    //clear la derniere ligne
    for (int i = 0; i < 80; i++)
        p[80*24+i] = (WHITE_ON_BLACK << 8) | ' ';
}

int	count_int(int nb)
{
	int		size;

	size = 0;
	while (nb >= 10)
	{
		nb /= 10;
		size++;
	}
	size++;
	return (size);
}

void	affichage(int mask, int nb)
{
	int		digit;

	while (mask > 0)
	{
		digit = (nb / mask) % 10 + '0';
        write_char(digit);
		mask /= 10;
	}
}

void	ft_putnbr(int nb)
{
	int		size;
	int		mask;
	int		index;

	if (nb == -2147483648)
	{
		kprint("-2147483648");
		return ;
	}
	if (nb < 0)
	{
		kprint("-");
		nb = -nb;
	}
	size = count_int(nb);
	mask = 1;
	index = 1;
	while (index < size)
	{
		mask *= 10;
		index++;
	}
	affichage(mask, nb);
}