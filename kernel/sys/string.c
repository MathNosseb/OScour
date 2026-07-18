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

char *strcpy(char *destination, char *source)
{
    int i;

    i = 0;
    while (source[i])
    {
        destination[i] = source[i];
        i++;
    }
    return (destination);    
}

void int_to_char(int nbr, char *texte)
{
    if (nbr == 0) {
        texte[0] = '0';
        texte[1] = '\0';
        return;
    }

    int index = 0;

    if (nbr < 0) {
        texte[index++] = '-';
        nbr = -nbr;
    }

    while (nbr > 0) {
        texte[index++] = '0' + (nbr % 10);
        nbr /= 10;
    }
    texte[index] = '\0';

    reverse_string(texte + (texte[0] == '-' ? 1 : 0));
}

int char_to_int(char *texte)
{
    int result = 0;
    int i = 0;

    while (texte[i] != '\0')
    {
        result = result * 10 + (texte[i] - '0');
        i++;
    }

    return result;
}

int strlen(char *texte)
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