#include "interuption.h"

void	load_lidt(struct IDTR *);
struct descriptor   idt[256];

void	init_entry(void)
{
	int                 i;

    i = 0;
    while (i < 256)
    {
    	idt[i].selector = 0x08;
		idt[i].reserved = 0;
		idt[i].attributs = 0b10001110;
		i++;
	}
}

void    set_entry(int handle, uint32_t adr)
{
	if (handle < 0 || handle > 255)
		return ;
	idt[handle].high_offset = (adr >> 16) & 0xFFFF;
	idt[handle].low_offset = adr & 0xFFFF;
}

void	load_idt(void)
{
	struct IDTR	idtr;

	idtr.size = 2047;//(256 * 8 - 1)
	idtr.base = (uint32_t)&idt;
	load_lidt(&idtr);
}
