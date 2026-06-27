#pragma once
#include "../sys/struct.h"
#include "../sys/string.h"
#include "../sys/hex.h"
#include "../output/vga.h"
void read_ata(int disk_num, uint32_t lba, int sec_number);