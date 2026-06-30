#pragma once
#include "../sys/struct.h"
#include "../output/vga.h"
#include "../sys/string.h"
#include "../sys/hex.h"
#include "../mem/mem.h"

void load_program(void *adr, int size);
void run_program(void *adr);
