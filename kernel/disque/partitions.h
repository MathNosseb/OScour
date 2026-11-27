#pragma once
#include "disk.h"

struct  partition
{
	uint8_t		flag;
	uint8_t		start_high_chs;
	uint16_t	start_low_chs;
	uint8_t		partition_type;
	uint8_t		end_high_chs;
	uint16_t	end_low_chs;
	uint32_t	start_lba;
	uint32_t	end_lba;
}__attribute__((packed));

void				put_partitions(void);
struct partition	*get_partitions(int disk_num);
