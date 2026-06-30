#include "shell.h"
#include "../mem/mem.h"
#include "../sys/hex.h"
#include "../sys/struct.h"
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
        vga_putchar("\n");
        free(argv); 
        return;
    } 

    
    int reconnu = 0;

    if (compare_word_buff("man", argv[0]))
    {
        reconnu = 1;
        free(argv);
        vga_putchar("\n");
        system_functions_descriptions(argc, argv);
        return;
    }
    if (compare_word_buff("shutdown", argv[0]))
    {
        outw(0x604, 0x2000);//qemu
        outw(0x4004, 0x3400);//virtualbox
    }

    if (compare_word_buff("clear", argv[0]))
    {
        clear_screen();
        reconnu = 1;
        free(argv);
        return;
    }
    if (compare_word_buff("info", argv[0]))
    {
        vga_putchar("\n");
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
        vga_putchar("\ndone");
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
    if (compare_word_buff("monitor", argv[0]))
    {
        free(argv);
        reconnu = 1;
        //affiche les ressources de l'ordinateur

        //les ressources
        uint64_t total_mem = (get_total_ram());
        int heap_mem = get_heap_ram_usage();
        int stack_mem = get_stack_ram_usage();
        int heap_total_mem = get_heap_ram_usage_and_non_use();

        //les textes
        char heap_quantity[11];
        char stack_quantity[11];
        char total_ram[11];
        char total_heap[11];

        //mettre les valeurs dans les textes
        int_to_char(heap_mem, heap_quantity);//quantité de heap utilisé
        int_to_char(stack_mem, stack_quantity);//quantité de stack utilisé
        int_to_char(total_mem, total_ram);//quantité de ram dans l ordi
        int_to_char(heap_total_mem, total_heap);//distance depuis la derniere adresse de heap avec le start

        //print
        vga_putchar("\n");
        vga_putchar("ressources ram: "); vga_putchar(total_ram);vga_putchar(" Octets");vga_putchar("\n");
        vga_putchar("heap usage: "); vga_putchar(heap_quantity);vga_putchar(" Octets");vga_putchar("\n");
        vga_putchar("stack usage: "); vga_putchar(stack_quantity);vga_putchar(" Octets");vga_putchar("\n");
        vga_putchar("stack deployment: "); vga_putchar(total_heap);vga_putchar(" Octets");vga_putchar("\n");

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
    }

    if (compare_word_buff("run", argv[0]))
    {
        if (argc < 2)
        {
            free(argv); 
            return;
        } 
        //recuperer l adresse
        vga_putchar("\n");
        run_program((uint32_t *)hex_to_int(argv[1]));
        free(argv);
        reconnu = 1;
        

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

/// @brief detail les commandes -> commande man
/// @param argv la commande split en liste de mots
void system_functions_descriptions(int argc, char **argv)
{
    if (argc >= 2)
    {
        //demande le detail d'une fonction
        if (compare_word_buff("man", argv[1]))
        {
            vga_putchar("INCEPTION\n");
            vga_putchar("affiche les details d'une fonction systeme\n");
        }
        else if (compare_word_buff("clear", argv[1]))
        {
            vga_putchar("efface l'ecran\n");
        }else if(compare_word_buff("info", argv[1]))
        {
            vga_putchar("affiche les infos systeme\n");
        }else if(compare_word_buff("echo", argv[1]))
        {
            vga_putchar("affiche du texte a l'ecran\n");
            vga_putchar("les parametres sont tous affiches\n");
        }else if(compare_word_buff("allocate", argv[1]))
        {
            vga_putchar("alloue une certaine quantite de memoire en octet dans la heap\n");
            vga_putchar("prend un parametre qui est un nombre en octet\n");
            vga_putchar("affiche \"done\" quand il a fini\n");
        }else if(compare_word_buff("allocate", argv[1]))
        {
            vga_putchar("reserve une quantite de memoire dans la heap\n");   
            vga_putchar("prend en parametre la quantite en octet de ram a allouer\n");
        }else if(compare_word_buff("free", argv[1]))
        {
            vga_putchar("libere la ram alloue avec allocate\n");
            vga_putchar("prend l adresse de la donnee\n");
        }else if(compare_word_buff("dump", argv[1]))
        {
            vga_putchar("affiche les blocs memoire alloue libre ou non\n");
            vga_putchar("permet de connaitre l'adresse des blocs\n");
        }else if(compare_word_buff("monitor", argv[1]))
        {
            vga_putchar("affiche les ressources de l'ordinateur\n");
        }else if(compare_word_buff("load", argv[1]))
        {
            vga_putchar("charge depuis le primary disque dans la heap\n");
            vga_putchar("1er parametre: adresse sur le disque");
            vga_putchar("2eme parametre: taille de la donnee en octet");
            vga_putchar("affiche l'adresse dansizes la heap du debut de la donnee\n");
            vga_putchar("affiche en hexadecimal les donnees stockes\n");
        }else if(compare_word_buff("run", argv[1]))
        {
            vga_putchar("lance un programme qui se trouve dans la heap\n");
            vga_putchar("1er parametre: adresse du programme dans la heap\n");
            vga_putchar("saute au programme comme une fonction standart\n");
        }else if(compare_word_buff("ata", argv[1]))
        {
            vga_putchar("affiche des donnees dans le disque / les charges dans la ram\n");
            vga_putchar("1er param -> read or load\n");
            vga_putchar("2eme param -> numero du disque (1 pour le slave et 0 pour le primaire)\n");
            vga_putchar("3eme param -> adresse lba\n");
            vga_putchar("4eme param -> quantite de secteur a lire ou charger\n");
        }else if(compare_word_buff("shutdown", argv[1])){
            vga_putchar("eteint l'ordinateur\n");
            vga_putchar("fonctionne sur VirtualBox et Qemu\n");
        }else{
            vga_putchar("la commande n'existe pas\n");
        }
    }else{
        //detail de toutes les fonctions systèmes
        vga_putchar("man -> donne les details des programmes\n");
        vga_putchar("clear -> efface l'ecran\n");
        vga_putchar("info -> infos systeme\n");
        vga_putchar("echo -> affiche du texte\n");
        vga_putchar("allocate -> alloue de la memoire\n");
        vga_putchar("free -> libere de la memoire\n");
        vga_putchar("dump -> affiche le contenu de la memoire\n");
        vga_putchar("monitor -> affiche les ressources\n");
        vga_putchar("load -> charge depuis le primary disque dans la heap\n");
        vga_putchar("run -> lance un programme situe dans la ram\n");
        vga_putchar("ata -> lit un disque ou charge un disque en memoire\n");
        vga_putchar("shutdown -> arret de l'ordinateur\n");
    }
    
}