#include "interuption.h"
#include "list_interupt.h"

void				load_lidt(struct IDTR *);
struct descriptor	idt[256];

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
	set_entry(0, (uint32_t)div_0);
	set_entry(5, (uint32_t)range_exceed);
	set_entry(6, (uint32_t)invalid_instruction);
	set_entry(8, (uint32_t)double_fault);
	set_entry(10, (uint32_t)invalid_tss);
	set_entry(12, (uint32_t)stack_fault);
	set_entry(13, (uint32_t)protection_fault);
	set_entry(14, (uint32_t)page_fault);
	set_entry(17, (uint32_t)alignement_check);
	set_entry(21, (uint32_t)control_protection);
	load_idt();
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
