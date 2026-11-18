#include "vga.h"

static uint16_t  g_cusor_pos = 0;

/// @brief affiche une suite de caracteres a l ecran -> doit rester en permier pour avoir la meme adr
/// @param texte le texte a afficher sous forme de string ""
void    kprint(char *texte)
{
    kprint_color(texte, WHITE_ON_BLACK);
}

void print_at(int x, int y, char *text)
{
    volatile uint16_t*  p;
    int                 index;

    index = 0;
    p = (volatile uint16_t *)VIDEO_MEMORY;
    while (text[index] != '\0')
    {
        p[y * 80 + x + index] = (GREEN_ON_BLACK << 8) | text[index];
        index++;
    }
}

void    write(char *s1, int len)
{
    char    *s2;
    int     i;

    i = 0;
    s2 = allocate(len + 1);
    if (!s2)
        return;
    while (i < len)
    {
        s2[i] = s1[i];
        i++;
    }
    s2[i] = '\0';
    kprint_color(s2, WHITE_ON_BLACK);
    free(s2);
}

void    kput_number(int nb)
{
    char buf[12];

    itoa(nb, buf);
    kprint(buf);
}

void    kprint_color(char *texte, uint8_t color)
{
    volatile uint16_t   *p;
    int                 index;

    index = 0;
    p = (volatile uint16_t *)VIDEO_MEMORY;
    while (texte[index] != '\0')
    {
        char c = texte[index++];

        if (c == '\n')
            g_cusor_pos = (g_cusor_pos / VGA_COLS + 1) * VGA_COLS;
        else if (c == '\b')
        {
            //backspace
            if (g_cusor_pos > 0)
                g_cusor_pos--;
            p[g_cusor_pos] = (color << 8) | ' ';
        }
        else if (c == '\t')
            g_cusor_pos += 4 - (g_cusor_pos % 4);
        else
        {
            p[g_cusor_pos] = (color << 8) | c;
            g_cusor_pos++;
        }
        while (g_cusor_pos >= VGA_COLS * VGA_ROWS)
        {
            scroll_screen();
            g_cusor_pos -= VGA_COLS;
        }
    }
    update_cursor();
}

void    update_cursor(void)
{
    outb(VGA_CRTC_ADDR, 0x0F);
    outb(VGA_CRTC_DATA, (uint8_t)(g_cusor_pos & 0xFF));
    outb(VGA_CRTC_ADDR, 0x0E);
    outb(VGA_CRTC_DATA, (uint8_t)(g_cusor_pos >> 8));
}

void    clear_screen(void)
{
    volatile uint16_t   *p;
    int                 i;

    i = 0;
    p = (volatile uint16_t *)VIDEO_MEMORY;
    while (i < VGA_COLS * VGA_ROWS)
        p[i++] = (WHITE_ON_BLACK << 8) | ' ';
    set_cursor(0);
    update_cursor();
}

void    scroll_screen(void)
{
    //deplace toutes les lignes vers le haut sauf la première
    //pour toutes les lignes faire -80 a leur position
    //ligne = caractere + couleur -> 2 octets
    //chaque ligne = 160 octets
    volatile uint16_t   *p;
    uint8_t             j;
    uint8_t             i;

    j = 0;
    p = (volatile uint16_t *)VIDEO_MEMORY;
    //on copie du bas vers le haut
    while (j < VGA_ROWS - 1)
    {
        i = 0;
        while (i < VGA_COLS)
        {
            p[j * 80 + i] = p[(j + 1) * 80 + i];
            i++;
        }
        j++;
    }
    i = 0;
    //clear la derniere ligne
    while (i < VGA_COLS)
    {
        p[80*24+i] = (WHITE_ON_BLACK << 8) | ' ';
        i++;
    }
}

void    set_cursor(int position)
{
    g_cusor_pos = position;
    update_cursor();
}

int get_cursor(void)
{
    return (g_cusor_pos);
}