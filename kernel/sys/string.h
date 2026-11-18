#pragma once
void    reverse_string(char *);
char    *itoa(int nbr, char *str);
char    *replace_char(char *str, char from, char by);
char    **split_words(char *str, int *count);
int     ft_atoi(char *str);
char    *strcpy(char *destination, char *source);
int     strlen(char *texte);
int     strcmp(const char *s1, const char *s2);
void    memset(void *destination, char data, unsigned int size);