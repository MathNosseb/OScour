#include "vga.h"
#include "struct.h"
#include "keyboard.h"
#include "shell.h"


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
    vga_puchar_color("root@local:", GREEN_ON_BLACK);
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













