#pragma once
#include "../sys/struct.h"
#include "../sys/string.h"
#include "../sys/hex.h"
#include "../output/vga.h"
#include "../mem/mem.h"

void		read_ata(int disk_num, uint32_t lba, int sec_number);
uint32_t	*load_ata(int disk_num, uint32_t lba, int sec_number);
void		write_ata(int disk_num, uint32_t lba, uint8_t *data, uint32_t size);
