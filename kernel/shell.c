#include "shell.h"

char sanitized_command[256];

void detect_command()
{
    //char * command = sanitize_command(get_command());
    
    int reconnu = 0;

    if (compare_word_buff("clear", get_command()))
    {
        clear_screen();
        reconnu = 1;
    }
    if (compare_word_buff("info", get_command()))
    {
        clear_screen();
        vga_putchar("   ____   _____                             ___  \n");
        vga_putchar("  / __ \\ / ____|                           |__ \\ \n");
        vga_putchar(" | |  | | (___   ___ ___  _   _ _ __  __   __ ) |\n");
        vga_putchar(" | |  | |\\___ \\ / __/ _ \\| | | | '__| \\ \\ / // / \n");
        vga_putchar(" | |__| |____) | (_| (_) | |_| | |     \\ V // /_ \n");
        vga_putchar("  \\____/|_____/ \\___\\___/ \\__,_|_|      \\_/|____|\n");
        vga_puchar_color("Oscour made by mathnosseb v2.0.1", CYAN_ON_BLACK);

        reconnu = 1;
    }
    if (compare_first_word_buffer("echo ", get_command()))
    {
        vga_putchar("\n");
        //vga_putchar(del_prefix(&get_command));
        reconnu = 1;
    }

    if (!reconnu) vga_puchar_color("\ncommande non reconnu", RED_ON_BLACK);
    
    vga_putchar("\n");

}

int compare_word_buff(char *word, char *buffer)
{
    int index = 0;
    while (word[index] != '\0')
    {
        if (word[index] != buffer[index]) return 0;
        index++;
    }
    return buffer[index] == '\0';
}

int compare_first_word_buffer(char *word, char *buffer)
{
    int index = 0;
    while (word[index] != ' ')
    {
        if (word[index] != buffer[index])
        {
            return 0;
        }
        index++; 
    }
    return 1;
    
}

char *del_prefix(char *buffer)
{
    clear_buffer(sanitized_command,256);
    int index = 0;
    int sanitize_index = 0;
    int valid = 0;
    int len_buffer = 0;
    while (buffer[len_buffer] != '\0')
    {
        len_buffer++;
    }
    
    while (!valid)
    {
        if (buffer[index] == ' ')
        {
            valid == 1;
        }
        index++;
        
    }

    for (int i = 0; i < len_buffer-index; i++)
    {
        sanitized_command[sanitize_index] = buffer[len_buffer - index + i];
    }
    
    return sanitized_command;

}