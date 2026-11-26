#pragma once
#include "../sys/struct.h"
#include "../sys/string.h"
#include "../sys/hex.h"
#include "../output/vga.h"
#include "../mem/mem.h"
#include "partitions.h"

void		read_ata(int disk_num, uint32_t lba, int sec_number);
void        *load_ata(int disk_num, uint32_t lba, int sec_number);
void		write_ata(int disk_num, uint32_t lba, uint8_t *data, uint32_t size);
void        write_ata_at(int disk_num, uint64_t offset, uint8_t *data, uint32_t size);

void    hello_world(uint32_t lba);
