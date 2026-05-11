#include "output/vga.h"
#include "sys/struct.h"
#include "sys/string.h"
#include "sys/hex.h"
#include "input/keyboard.h"
#include "app/shell.h"
#include "mem/mem.h"

void _start() {

    vga_puchar_color("\n                                   Boot Done\n",GREEN_ON_BLACK);
    vga_putchar("                                    Welcome\n");
    vga_putchar("   ____   _____                             ___  \n");
    vga_putchar("  / __ \\ / ____|                           |__ \\ \n");
    vga_putchar(" | |  | | (___   ___ ___  _   _ _ __  __   __ ) |\n");
    vga_putchar(" | |  | |\\___ \\ / __/ _ \\| | | | '__| \\ \\ / // / \n");
    vga_putchar(" | |__| |____) | (_| (_) | |_| | |     \\ V // /_ \n");
    vga_putchar("  \\____/|_____/ \\___\\___/ \\__,_|_|      \\_/|____|\n");
    vga_puchar_color("Oscour made by mathnosseb v2.0.1\n", CYAN_ON_BLACK);
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
    
    char value[11];
    char *adr = allocate(5);
    adr[0] = 'A';
    adr[1] = '\0';
    int_to_hex((uint32_t)adr, value); 
    vga_putchar(value);

    int *adr2 = allocate(100);
    int_to_hex((uint32_t)adr2, value);
    vga_putchar(value);
    

    while (1) {
        update_cursor();

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
