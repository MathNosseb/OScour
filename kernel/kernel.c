#include "output/vga.h"
#include "sys/struct.h"
#include "sys/string.h"
#include "sys/hex.h"
#include "input/keyboard.h"
#include "app/shell.h"
#include "mem/mem.h"

void _start() {
    load_art();
    
    char secteurs[1];
    char stack[7];
    int nbr_secteur = *(int *)0x500; //nombre de secteurs chargé
    uint32_t kernel_size = nbr_secteur * 512;
    uint32_t emplacement = kernel_size + 0x1000;
    uint32_t stack_size = 0x80000 - emplacement;
    int_to_char(nbr_secteur,secteurs);
    int_to_char(stack_size, stack);
    vga_puchar_color(secteurs, CYAN_ON_BLACK); vga_puchar_color(" secteurs, ", CYAN_ON_BLACK); 
    vga_puchar_color(stack, CYAN_ON_BLACK); vga_puchar_color(" Octets de stack dispo", CYAN_ON_BLACK); 
    
    vga_puchar_color("\nroot@local:", GREEN_ON_BLACK);
    uint64_t total_mem = (get_total_ram()/1024/1024);

    while (1) {
        //OUTPUT
        update_cursor();
        
        char heap_quantity[9];
        char stack_quantity[9];
        char total_ram[9];
        int heap_mem = get_heap_ram_usage();
        int stack_mem = get_stack_ram_usage();
        
        int_to_char(heap_mem, heap_quantity);
        int_to_char(stack_mem, stack_quantity);
        int_to_char(total_mem, total_ram);
        print_at(54,0, "heap "); print_at(60,0,heap_quantity); print_at(64,0, " octets");
        print_at(54,1, "stack "); print_at(60,1,stack_quantity); print_at(64,1, " octets");
        print_at(54,2, "total "); print_at(60,2,total_ram); print_at(64,2, " Mo");


        //INPUT + COMMAND
        if (detect_buffer())
        {
            uint8_t scancode = get_scancode();
            if (scancode != 0xFF)
            {
                if (scancode == 0x1C)
                {
                    detect_command();
                    vga_puchar_color("root@local:", GREEN_ON_BLACK);
                }
                vga_putchar(scanCodeToChar(scancode));
            }
            
        }        
    }
}