#include "compiler.h"

//va compiler et placer dans la memoire les opcodes

//compile dans le buffer le programme et retourne la taille
int assemble_line(unsigned char *buff, unsigned char *line)
{
	if (!strcmp(line, "ret"))
		buff
}

void	write_bin(unsigned char *buff, int size)
{
	unsigned char *p = allocate(size);
	int i = 0;
	while (i < size)
	{
		p[i] = buff[i];
	}
}
