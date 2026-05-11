#include "string.h"
#include "struct.h"
#include "../mem/mem.h"
#include "../output/vga.h"
void reverse_string(char *str) {
    int i = 0;
    int j = 0;

    while (str[j] != '\0')
        j++;

    j--;

    while (i < j) {
        char tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
        i++;
        j--;
    }
}


void int_to_char(int nbr, char *texte)
{
    //bug, inverse les nombres
    int index = 0;
    while (nbr > 0) {
        int digit = nbr % 10; // dernier chiffre
        nbr /= 10;

        char nbr_text;
        switch (digit)
        {
            case 0: nbr_text = '0'; break;
            case 1: nbr_text = '1'; break;
            case 2: nbr_text = '2'; break;
            case 3: nbr_text = '3'; break;
            case 4: nbr_text = '4'; break;
            case 5: nbr_text = '5'; break;
            case 6: nbr_text = '6'; break;
            case 7: nbr_text = '7'; break;
            case 8: nbr_text = '8'; break;
            case 9: nbr_text = '9'; break;
            default: break;
        }

        texte[index] = nbr_text;
        index++;
    }

    reverse_string(texte);

}

int len_string(char *texte)
{
    int index = 0;
    while (texte[index] != 0)
    {
        index++;
    }
    return index;    
}

char *replace_char(char *str, char from, char by)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == from)
            str[i] = by;
    }
    return str;
}

char **split_words(char *str, int *count)
{
    int i = 0;
    int words = 0;

    // compter
    while (str[i])
    {
        if (str[i] != ' ' && (i == 0 || str[i - 1] == ' '))
            words++;
        i++;
    }

    *count = words;

    char **mot = allocate(sizeof(char *) * words);

    i = 0;
    int idx = 0;

    while (str[i])
    {
        // skip spaces
        while (str[i] == ' ')
            i++;

        if (!str[i])
            break;

        mot[idx++] = &str[i];

        while (str[i] && str[i] != ' ')
            i++;

        if (str[i] == ' ')
        {
            str[i] = '\0';
            i++;
        }
    }

    return mot;
}