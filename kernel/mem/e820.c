#include "mem.h"
#include "e820.h"


uint64_t    get_total_ram(void)
{
    struct e820_entry *entries = (struct e820_entry *)0x8004;
    uint16_t count = *(uint16_t *)0x8000;

    uint64_t total = 0;

    for (int i = 0; i < count; i++)
    {
        total += entries[i].length;
    }

    return total;
}

void    analyse_mem(void)
{
    struct e820_entry *entries = (struct e820_entry *)0x8004;
    uint16_t count = *(uint16_t *)0x8000;
    struct e820_entry biggest;
    biggest.type = 2;
    biggest.length = 0;

    uint32_t tot = 0;
    for (int i = 0; i < count; i++)
    {
        if (entries[i].base != 0 && entries[i].type == 1 && entries[i].length > biggest.length)
            biggest = entries[i];
        char base[16];
        int_to_hex(entries[i].base, base);
        kprint("0x");
        kprint(base); kprint (" ");
        kput_number(entries[i].length/1024);
        kprint("Ko ");
        kput_number(entries[i].type);
        if (entries[i].type == 1 && i != 0)
            kprint_color(" usable\n", GREEN_ON_BLACK);
        else
            kprint_color(" reserved\n", RED_ON_BLACK);
        tot += entries[i].length;
    }
    if (biggest.type == 2)
        KERNEL_PANIC("pas assez de ram");
    else
    {
        actual_adr = biggest.base;
        heap_start = biggest.base;
        heap_max = biggest.length;
        kput_number(tot/1024);
        kprint("Ko de ram total\n");
    }
}

uint32_t get_max_heap_size(void)
{
    struct e820_entry *entries = (struct e820_entry *)ENTRIE_START;
    uint16_t count = *(uint16_t *)ENTRIE_COUNT;
    struct e820_entry heap;
    heap.length = 0;

    for (int i = 0; i < count; i++)
    {
        if (entries[i].base != 0 && entries[i].type == 1 && entries[i].length > heap.length)
            heap = entries[i];
    }

    return (heap.length);
}