#include "shell.h"
#include "../mem/mem.h"
#include "../sys/hex.h"
#include "../disque/disk.h"
#include "programs.h"

char sanitized_command[256];

void detect_command()
{
    char *command = get_command();
    int argc;
    char **argv = split_words(command, &argc);

    if (argc == 0)
    {
        free(argv); 
        return;
    } 

    
    int reconnu = 0;

    if (compare_word_buff("clear", argv[0]))
    {
        clear_screen();
        reconnu = 1;
    }
    if (compare_word_buff("info", argv[0]))
    {
        clear_screen();
        load_art();

        reconnu = 1;
    }
    if (compare_word_buff("echo", argv[0]))
    {
        vga_putchar("\n");
        for (int index = 0; index < argc - 1; index++)
        {
            vga_putchar(argv[index + 1]);
            vga_putchar(" ");
        }
        reconnu = 1;
    }
    if (compare_word_buff("allocate", argv[0]))
    {
        if (argc < 2)
        {
            free(argv); 
            return;
        }
        
        char adresse[9];
        //on aloue la ram a la valeur de argv[1]
        int ram = char_to_int(argv[1]);
        uint32_t *p = allocate(ram);
        int_to_char((uint32_t)p, adresse);
        vga_putchar("\n"); vga_putchar(adresse);
        reconnu = 1;
    }
    if (compare_word_buff("free", argv[0]))
    {
        if (argc < 2)
        {
            free(argv); 
            return;
        } 
        
        free((uint32_t *)hex_to_int(argv[1]));
        reconnu = 1;
    }
    if (compare_word_buff("dump", argv[0]))
    {
        free(argv);//on clean la memoire pour pas avoir de residus de commandes
        dump_heap();
        reconnu = 1;
        return;
    }
    if (compare_word_buff("load", argv[0]))
    {
        if (argc < 3)
        {
            free(argv); 
            return;
        } 
        //recuperer l adresse
        vga_putchar("\n");
        load_program((uint32_t *)hex_to_int(argv[1]), char_to_int(argv[2]));
        free(argv);
        reconnu = 1;
        return;
        

    }

    if (compare_word_buff("run", argv[0]))
    {
        if (argc < 3)
        {
            free(argv); 
            return;
        } 
        //recuperer l adresse
        vga_putchar("\n");
        run_program((uint32_t *)hex_to_int(argv[1]), char_to_int(argv[2]));
        free(argv);
        reconnu = 1;
        return;
        

    }
    if (compare_word_buff("ata", argv[0]))
    {
        if (argc < 5)
        {
            free(argv); 
            return;
        } 

        vga_putchar("\n");
        int disk_num = char_to_int(argv[2]);
        uint32_t lba = char_to_int(argv[3]);
        int secteur_num = char_to_int(argv[4]);

        if(compare_word_buff("read", argv[1]))
        {
            reconnu = 1;
            read_ata(disk_num, lba, secteur_num);
        }else if (compare_word_buff("load", argv[1]))
        {
            reconnu = 1;
            load_ata(disk_num, lba, secteur_num);
        }     

        free(argv);
        
    }

    if (!reconnu) vga_puchar_color("\ncommande non reconnu", RED_ON_BLACK);
    
    vga_putchar("\n");
    free(argv);
    

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
    int i = 0;

    // avancer jusqu'à l'espace ou fin de chaîne
    while (buffer[i] != ' ' && buffer[i] != '\0')
        i++;

    // si on est sur un espace, le sauter
    if (buffer[i] == ' ')
        i++;

    // retourne l'adresse du reste
    return &buffer[i];


}