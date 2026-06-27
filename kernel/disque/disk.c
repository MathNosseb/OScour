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
        vga_putchar(value_text); vga_putchar(" ");
    }

    free(adr);

}

/// @brief charge le contenu du disque en mémoire (heap)
/// @param disk_num le numero du disque (0 master, 1 slave)
/// @param lba l adresse lba
/// @param sec_number le nombre de secteur a lire
/// @return l'adresse du contenu du disque dans la ram
uint32_t *load_ata(int disk_num, uint32_t lba, int sec_number)
{
    while (inb(0x1F7) & 0x80);  // attendre BSY=0

    outb(0x1F2, sec_number); //lire sec_number de secteurs

    outb(0x1F3, (uint8_t)lba);//debut fichier
    outb(0x1F4, (uint8_t)(lba >> 8));//pas d offset
    outb(0x1F5, (uint8_t)(lba >> 16));//pas d offset

    uint8_t last = 0;

    last |= (1 << 7);
    last |= (1 << 6);
    last |= (1 << 5);
    last |= (disk_num << 4);//slave

    last |= ((lba >> 24) & 0x0F);

    outb(0x1F6, last);
    outb(0x1F7, 0x20);  // commande READ SECTORS

    // Attendre DRQ=1 (données prêtes)
    while (!(inb(0x1F7) & 0x08));

    uint8_t *adr = allocate(sec_number * 512);

    for (int i = 0; i < sec_number * 256; i++) {
        uint16_t val = inw(0x1F0);
        adr[i * 2]     = (char)(val & 0xFF);
        adr[i * 2 + 1] = (char)((val >> 8) & 0xFF);
    }

    return (uint32_t *)adr;
}