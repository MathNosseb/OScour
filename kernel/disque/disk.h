#pragma once
#include "../sys/struct.h"
#include "../sys/string.h"
#include "../sys/hex.h"
#include "../output/vga.h"
#include "../mem/mem.h"
#include "partitions.h"

struct	t_disk_info
{
	int					disk_number;
	char				*name;
	uint32_t			sector_number;
	struct partition	*part;
};

void		read_ata(int disk_num, uint32_t lba, int sec_number);
void        *load_ata(int disk_num, uint32_t lba, int sec_number);
void		write_ata(int disk_num, uint32_t lba, uint8_t *data, uint32_t size);
void        write_ata_at(int disk_num, uint64_t offset, uint8_t *data, uint32_t size);

void    hello_world(uint32_t lba);

struct t_disk_info	*get_disk_info(int disk_num);
void				put_disk_infos(int disk_num);