#pragma once
#include "../sys/struct.h"

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