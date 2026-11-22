#pragma once
#include "../sys/struct.h"
#include "../app/banner.h"
#include "e820.h"

#define FUNCTIONS_TABLE 0x504
#define SECTORS_NUMBER 0x500
#define STACK_EMPLACEMENT 0x80000

struct heap
{
    uint32_t canarie;
    int size;//taille en octet de la zone
    uint32_t end;//adresse de fin
    int free;//0 -> libre, 1 -> occupé
    struct heap *precedent;
    struct heap *suivant;
    int flag;//flag si suivant est present
};

extern  uint32_t    actual_adr;
extern  uint32_t    heap_start;
extern  uint32_t    heap_max;
extern  struct heap *precend_heap;

void        KERNEL_PANIC(const char *s);
void        init_memory_info(void);
void        *allocate(int);
uint32_t    get_stack_usage();
uint32_t    get_heap_ram_usage_and_non_use();
void        heap_stress_test();
int         get_heap_ram_usage();
void        free(void *adr);
void        dump_heap();
void        init_memory();
uint32_t    get_max_heap_size();
uint32_t    get_mem_usage();