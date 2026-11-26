#pragma once
#include "struct.h"
void    reverse_string(char *);
char    *itoa(int nbr, char *str);
char    *replace_char(char *str, char from, char by);
char    **split_words(char *str, int *count);
int     atoi(char *str);
char    *strcpy(char *destination, char *source);
int     str_len(char *texte);
int     strcmp(const char *s1, const char *s2);
void    *memset(void *p, int data, unsigned int size);
void	put_size(uint64_t size);