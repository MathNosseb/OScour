#include "partitions.h"

void	put_partitions(void)
{
	kprint("\n");
	struct partition	*part;
	int					i;

	i = 0;
	//les partitions sont stocké dans le bootloader
	part = get_partitions();
	if (!part)
		return ;
	while (i < 4)
	{
		if ((part[i]).flag == 0x80)
		{
			kprint("partition valide de ");
			put_size(((part[i]).end_lba - (part[i]).start_lba) * 512);
			kprint("\n");
		}
		else
			kprint("partition invalide\n");
		i++;
	}
	free(part);
}

struct partition	*get_partitions(void)
{
	struct partition	*part;
	uint8_t				*adr;
	int					i;

	adr = load_ata(0, 0, 1);
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