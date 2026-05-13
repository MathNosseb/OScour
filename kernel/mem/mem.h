#include "../sys/struct.h"
#define HEAP_START 0x100000



void *allocate(int);
uint32_t get_stack_ram_usage();
int get_heap_ram_usage();
uint64_t get_total_ram();
void free(void *adr);
void dump_heap();
void init_memory();