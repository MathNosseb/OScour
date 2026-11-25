#include "programs.h"
#include "../sys/struct.h"
#include "../output/vga.h"
#include "../sys/string.h"
#include "../sys/hex.h"         
#include "../mem/mem.h"
#include "../sys/list.h"

void run_program(void *adr)
{
	char pointeur[11];
	itoa((uint32_t)adr, pointeur);
	

	uint8_t *ptr = (uint8_t*)atoi(pointeur);

	kprint("\nrun program...\n");
	void (*func)() = (void (*)())ptr;
	func();
	kprint("\nend program...\n");
}