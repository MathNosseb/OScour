#pragma once
#include "../sys/struct.h"
#include "../app/banner.h"
#include "../output/vga.h"
#include "../sys/string.h"
#include "../sys/hex.h"
#include "e820.h"
#include "allocator.h"
#include "mem_info.h"

#define FUNCTIONS_TABLE 0x504
#define SECTORS_NUMBER 0x500
#define KERNEL_EMPLACEMENT 0x1000
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
void        heap_stress_test();
void        dump_heap();
void		mem_coalition(void);