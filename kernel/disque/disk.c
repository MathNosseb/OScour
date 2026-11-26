#include "disk.h"

/*
| Port  | Rôle                           |
| ----- | ------------------------------ |
| 0x1F0 | Data                           |
| 0x1F2 | Nombre de secteurs             |
| 0x1F3 | LBA 0 (bits de 0 à 7)          |
| 0x1F4 | LBA 1 (bits de 8 à 15)         |
| 0x1F5 | LBA 2 (bits de 16 à 23)        |
| 0x1F6 | Drive + LBA 3 (bits de 24 à 27 |
| 0x1F7 | Commande / Status              |

0x1F6 -> le bit 7 doit être à 1
0x1F6 -> le bit 6 doit être à 1 pour dire qu'on est en lba
0x1F6 -> le bit 4 doit être a 0 pour parler du master disque
0x1F6 -> le bit 4 doit être à 1 pour parler du slave disque

*/

/// @brief affiche le contenu du disque a une certaine position
/// @param disk_num le numero du disque (0 master, 1 slave)
/// @param lba l adresse lba
/// @param sec_number le nombre de secteur a lire
void read_ata(int disk_num, uint32_t lba, int sec_number)
{
	uint8_t *adr = (uint8_t *)load_ata(disk_num, lba, sec_number);

	for (int value = 0; value < sec_number * 512; value++)
	{
	    char value_text[3];
	    int_to_hex((uint8_t)adr[value], value_text);
	    kprint(value_text); kprint(" ");
	}

	free(adr);

}

void	write_ata_at(int disk_num, uint64_t offset, uint8_t *data, uint32_t size)
{
	uint8_t		*sector_data;
	uint32_t	i;
	uint32_t	done;

	done = 0;

	while (done < size)
	{
		sector_data = load_ata(disk_num, (offset + done) / 512, 1);
		if (!sector_data)
			return ;
		i = 0;
		while (done + i < size && (offset + done )% 512 + i < 512)
		{
			sector_data[(offset + done) % 512 + i] = data[done + i];
			i++;
		}
		write_ata(disk_num, (offset + done) / 512, sector_data, 512);
		done += i;
		free(sector_data);
	}
}


void	write_ata(int disk_num, uint32_t lba, uint8_t *data, uint32_t size)
{
	uint8_t		last;
	uint32_t	i;
	uint16_t	word;

	while (inb(0x1F7) & 0x80);
	last = 0;
	last |= (1 << 7);
	last |= (1 << 6);
	last |= (1 << 5);
	last |= (disk_num << 4);//slave
	last |= ((lba >> 24) & 0x0F);
	outb(0x1F6, last);
	outb(0x1F2, (size + 511) / 512);
	outb(0x1F3, (uint8_t)lba);
	outb(0x1F4, (uint8_t)(lba>>8));
	outb(0x1F5, (uint8_t)(lba>>16));
	outb(0x1F7, 0x30); //ecrire
	while (!(inb(0x1F7) & 0x08));//attendre que ca soit pret
	i = 0;
	while (i < size - 1)
	{
		word = data[i] | (data[i + 1] << 8);
		outw(0x1F0, word);
		i+=2;
	}
	if (size % 2 == 1)
	{
		word = data[i] | (0x00 << 8);
		outw(0x1F0, word);
		i++;
	}
	while (i < ((size + 511) / 512) * 512)
	{
		outw(0x1F0, 0);
		i += 2;
	}
	outb(0x1F7, 0xE7); //flush
	while (inb(0x1F7) & 0x80);
}

/// @brief charge le contenu du disque en mémoire (heap)
/// @param disk_num le numero du disque (0 master, 1 slave)
/// @param lba l adresse lba
/// @param sec_number le nombre de secteur a lire
/// @return l'adresse du contenu du disque dans la ram
void	*load_ata(int disk_num, uint32_t lba, int sec_number)
{
	while (inb(0x1F7) & 0x80);  // attendre BSY=0
	uint8_t last = 0;

	last |= (1 << 7);
	last |= (1 << 6);
	last |= (1 << 5);
	last |= (disk_num << 4);//slave

	last |= ((lba >> 24) & 0x0F);

	void	*adr = allocate(sec_number * 512);
	uint8_t	*data = (uint8_t *)adr;
	if (!adr)
		return (NULL);

	outb(0x1F6, last);

	outb(0x1F2, sec_number); //lire sec_number de secteurs

	outb(0x1F3, (uint8_t)lba);//debut fichier
	outb(0x1F4, (uint8_t)(lba >> 8));//pas d offset
	outb(0x1F5, (uint8_t)(lba >> 16));//pas d offset
	outb(0x1F7, 0x20);  // commande READ SECTORS

	for (int j = 0; j < sec_number; j++)
	{
		while (inb(0x1F7) & 0x80);
		while (!(inb(0x1F7) & 0x08));
		for (int i = 0; i < 256; i++)
		{
	    	uint16_t val = inw(0x1F0);
	    	data[j * 512 + i * 2]     = (char)(val & 0xFF);
	    	data[j * 512 + i * 2 + 1] = (char)((val >> 8) & 0xFF);
		}
	}		
	return (adr);
}
