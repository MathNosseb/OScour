#pragma once
#include "../sys/struct.h"
#include "../output/vga.h"
#include "../sys/string.h"
#include "../sys/hex.h"

#define ENTRIE_COUNT 0x8000
#define ENTRIE_START 0x8004

struct e820_entry
{
	uint64_t base;//Base address
	uint64_t length;// Length of "region" (if this value is 0, ignore the entry)
	uint32_t type;//Region "type"
	//Type 1: Usable (normal) RAM
	//Type 2: Reserved - unusable
	//Type 3: ACPI reclaimable memory
	//Type 4: ACPI NVS memory
	//ype 5: Area containing bad memory
	uint32_t acpi;
} __attribute__((packed));
//atribute packed sert a faire en sorte que en memoire les donnees prennent
//exactement la place demandé sans paging
//il ne va pas y avoir d espaces entre les element ca sert a donner une taille stricte
//entre chaqun des elements et comme je le recupere depuis le bios c est la taille
//litteral alors il ne faut vrm pas la changer

uint64_t    get_total_ram_size(void);
void        set_e820_entries(void);
uint32_t    get_heap_size(void);