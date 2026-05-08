#include "string.h"
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