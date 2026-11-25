#pragma once
#include "../struct.h"
#include "../../mem/mem.h"

void	init_entry(void);
void	set_entry(int handle, uint32_t adr);
void	load_idt(void);

struct descriptor
{
	uint16_t    low_offset;
	uint16_t    selector;
	uint8_t     reserved;
	uint8_t     attributs;
	// bit 7    : P (présent), à 1
	// bits 6-5 : DPL (0 à 3, 0 = le plus privilégié)
	// bit 4    : doit être 0
	// bits 3-0 : type (1110 = porte d'interruption 32 bits)
	uint16_t    high_offset;
} __attribute__((packed));

struct  IDTR
{
	uint16_t    size;
	uint32_t    base;
} __attribute__((packed));
