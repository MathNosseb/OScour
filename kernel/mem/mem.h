#pragma once
#include "../sys/struct.h"

#define FUNCTIONS_TABLE 0x504
#define SECTORS_NUMBER 0x500

struct heap
{
    uint32_t canarie;
    int size;//taille en octet de la zone
    uint32_t start;//adresse de debut
    uint32_t end;//adresse de fin
    int free;//0 -> libre, 1 -> occupé
    uint32_t precedent;
    
    uint32_t suivant;
    int flag;//flag si suivant est present
};

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

void	init_memory_info(void);
void *allocate(int);
uint32_t get_stack_ram_usage();
uint32_t get_heap_ram_usage_and_non_use();
void heap_stress_test();
int get_heap_ram_usage();
uint64_t get_total_ram();
void free(void *adr);
void dump_heap();
void init_memory();
void analyse_mem();
uint32_t get_max_heap_size();
uint32_t get_mem_usage();