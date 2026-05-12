#include "../sys/struct.h"
#define HEAP_START 0x100000

void *allocate(int);
uint32_t get_stack_ram_usage();
uint32_t detect_free_mem_space();
int get_heap_ram_usage();
void free(void *adr);