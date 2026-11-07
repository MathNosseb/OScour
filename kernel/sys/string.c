#include "string.h"
#include "struct.h"
#include "../mem/mem.h"
#include "../output/vga.h"

int strlen(char *str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);    
}

void reverse_string(char *str)
{
    int     size;
    int     i;
    char    temp;

    size = strlen(str);
    i = 0;
    while (i < size / 2)
    {
        temp = str[i];
        str[i] = str[size - i - 1];
        str[size - i - 1] = temp;
        i++;
    }
    str[size] = '\0';
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
    destination[i] = '\0';
    return (destination);    
}

int strcmp(char *s1, char *s2)
{
	int i = 0;
	while (s1[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]); 
}

char    *itoa(int nbr, char *str)
{
    int index;

    index = 0;
    if (nbr == 0) {
        str[0] = '0';
        str[1] = '\0';
        return (str);
    }
    if (nbr < 0) {
        str[index++] = '-';
        nbr = -nbr;
    }
    while (nbr > 0) {
        str[index++] = '0' + (nbr % 10);
        nbr /= 10;
    }
    str[index] = '\0';
    reverse_string(str + (str[0] == '-' ? 1 : 0));
    return (str);
}

int ft_atoi(char *str)
{
	int	nbr;
	int	negative;
	int	i;

	nbr = 0;
	negative = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	while (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			negative++;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		nbr = nbr * 10 + (str[i] - '0');
		i++;
	}
	if (negative % 2)
		return (-nbr);
	return (nbr);
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
    int i;
    int words_nbr;
    int idx;
    char **words;

    i = 0;
    words_nbr = 0;
    idx = 0;
    // compter
    while (str[i])
    {
        if (str[i] != ' ' && (i == 0 || str[i - 1] == ' '))
            words_nbr++;
        i++;
    }
    *count = words_nbr;
    words = allocate(sizeof(char *) * words_nbr);
    i = 0;
    while (str[i])
    {
        while (str[i] == ' ')
            i++;
        if (!str[i])
            break;
        words[idx++] = &str[i];
        while (str[i] && str[i] != ' ')
            i++;
        if (str[i] == ' ')
        {
            str[i] = '\0';
            i++;
        }
    }
    return (words);
}
