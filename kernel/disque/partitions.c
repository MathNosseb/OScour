#include "partitions.h"

void	put_partitions(struct partition *part)
{
	int	i;

	i = 0;
	if (!part)
		return ;
	kprint("partitions : ");
	while (i < 4)
	{
		if ((part[i]).flag == 0x80)
		{
			put_size((part[i]).sec_count * 512);
			kprint(", ");
		}
		i++;
	}
}

struct partition	*get_partitions(int disk_num)
{
	struct partition	*part;
	uint8_t				*adr;
	int					i;

	adr = load_ata(0, disk_num, 1);
	if (!adr)
		return (NULL);
	part = allocate(4 * sizeof(struct partition));
	if (!part)
	{
		free(adr);
		return (NULL);
	}
	i = 0;
	while (i < 4 * sizeof(struct partition))
	{
		((uint8_t *)part)[i] = adr[446 + i];
		i++;
	}
	free(adr);
	return (part);
}

struct partition *create_partition(int disk_num, uint32_t start_lba, uint32_t sec_count, int part_number)
{
	struct partition *part;
	
	if (part_number < 0 && part_number > 3)
		return (NULL);
	part = allocate(sizeof(struct partition));
	if (!part)
		return (NULL);
	part->flag = 0x80;
	part->start_high_chs = 0xFF;
	part->start_low_chs = 0xFF;
	part->partition_type = 0x0C;
	part->end_high_chs = 0xFF;
	part->end_low_chs = 0xFF;
	part->start_lba = start_lba;
	part->sec_count = sec_count;
	write_ata_at(disk_num, 446 + part_number * sizeof(struct partition), (uint8_t *)part, sizeof(struct partition));
	return (part);
}