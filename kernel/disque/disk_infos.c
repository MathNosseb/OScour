#include "disk.h"

void	put_disk_infos(int disk_num)
{
	struct t_disk_info *disk = get_disk_info(disk_num);
	if (!disk)
		return ;
	kprint("\n");
	kprint(disk->name);
	put_size(disk->sector_number * 512);
	kprint(" ");
	put_partitions(disk->part);
	free(disk->part);
	free(disk->name);
	free(disk);
}

struct t_disk_info	*get_disk_info(int disk_num)
{
	struct t_disk_info	*disk;
	uint16_t	drive[256];
	int			i;

	i = 0;
	disk = allocate(sizeof(struct t_disk_info));
	if (!disk)
		return (NULL);
	disk->name = allocate(41);
	if (!disk->name)
	{
		free(disk);
		return (NULL);
	}
	if (disk_num == 0)
		outb(0x1F6, 0b11100000);
	else
		outb(0x1F6, 0b11110000);
	outb(0x1F7, 0xEC); //ecrire
	while (inb(0x1F7) & 0x80);
	while (!(inb(0x1F7) & 0x08));
	while (i < 256)
		drive[i++] = inw(0x1F0);
	i = 0;
	while (i < 20)
	{
		disk->name[i * 2 + 1]     = drive[27 + i] & 0xFF;
		disk->name[i * 2] = drive[27 + i] >> 8;
		i++;
	}
	disk->name[i] = '\0';
	disk->sector_number = drive[60] | ((uint32_t)drive[61] << 16);
	disk->disk_number = disk_num;
	disk->part = get_partitions(disk_num);
	return (disk);
}