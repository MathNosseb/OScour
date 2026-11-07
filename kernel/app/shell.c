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
        kprint("\n");
        free(argv); 
        return;
    } 

    
    int reconnu = 0;

    if (compare_word_buff("man", argv[0]))
    {
        reconnu = 1;
        free(argv);
        kprint("\n");
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
        kprint("\n");
        load_art();

        reconnu = 1;
    }
    if (compare_word_buff("echo", argv[0]))
    {
        kprint("\n");
        for (int index = 0; index < argc - 1; index++)
        {
            kprint(argv[index + 1]);
            kprint(" ");
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
        //on aloue la ram a la valeur de argv[1]
        int ram = ft_atoi(argv[1]);
        uint32_t *p = allocate(ram);
        kprint("\ndone");
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
        uint64_t total_mem = (get_total_ram() / 1024);
        int heap_mem = get_heap_ram_usage();
        int stack_mem = get_stack_ram_usage();
        int heap_total_mem = get_heap_ram_usage_and_non_use();
        uint32_t tot = get_mem_usage() / 1024;
        
        //print
        kprint("\n");
        kprint("ressources ram: "); ft_putnbr(total_mem);kprint(" Ko");kprint("\n");
        kprint("heap usage: "); ft_putnbr(heap_mem);kprint(" Octets");kprint("\n");
        kprint("stack usage: "); ft_putnbr(stack_mem);kprint(" Octets");kprint("\n");
        kprint("heap deployment: "); ft_putnbr(heap_total_mem);kprint(" Octets");kprint("\n");
        ft_putnbr(tot); kprint("KO / "); ft_putnbr(total_mem); kprint("Ko\n");
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
        kprint("\n");
        load_program((uint32_t *)hex_to_int(argv[1]), ft_atoi(argv[2]));
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
        kprint("\n");
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

        kprint("\n");
        int disk_num = ft_atoi(argv[2]);
        uint32_t lba = ft_atoi(argv[3]);
        int secteur_num = ft_atoi(argv[4]);

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

    if (!reconnu) kprint_color("\ncommande non reconnu", RED_ON_BLACK);
    kprint("\n");

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
            kprint("INCEPTION\n");
            kprint("affiche les details d'une fonction systeme\n");
        }
        else if (compare_word_buff("clear", argv[1]))
        {
            kprint("efface l'ecran\n");
        }else if(compare_word_buff("info", argv[1]))
        {
            kprint("affiche les infos systeme\n");
        }else if(compare_word_buff("echo", argv[1]))
        {
            kprint("affiche du texte a l'ecran\n");
            kprint("les parametres sont tous affiches\n");
        }else if(compare_word_buff("allocate", argv[1]))
        {
            kprint("alloue une certaine quantite de memoire en octet dans la heap\n");
            kprint("prend un parametre qui est un nombre en octet\n");
            kprint("affiche \"done\" quand il a fini\n");
        }else if(compare_word_buff("allocate", argv[1]))
        {
            kprint("reserve une quantite de memoire dans la heap\n");   
            kprint("prend en parametre la quantite en octet de ram a allouer\n");
        }else if(compare_word_buff("free", argv[1]))
        {
            kprint("libere la ram alloue avec allocate\n");
            kprint("prend l adresse de la donnee\n");
        }else if(compare_word_buff("dump", argv[1]))
        {
            kprint("affiche les blocs memoire alloue libre ou non\n");
            kprint("permet de connaitre l'adresse des blocs\n");
        }else if(compare_word_buff("monitor", argv[1]))
        {
            kprint("affiche les ressources de l'ordinateur\n");
        }else if(compare_word_buff("load", argv[1]))
        {
            kprint("charge depuis le primary disque dans la heap\n");
            kprint("1er parametre: adresse sur le disque");
            kprint("2eme parametre: taille de la donnee en octet");
            kprint("affiche l'adresse dansizes la heap du debut de la donnee\n");
            kprint("affiche en hexadecimal les donnees stockes\n");
        }else if(compare_word_buff("run", argv[1]))
        {
            kprint("lance un programme qui se trouve dans la heap\n");
            kprint("1er parametre: adresse du programme dans la heap\n");
            kprint("saute au programme comme une fonction standart\n");
        }else if(compare_word_buff("ata", argv[1]))
        {
            kprint("affiche des donnees dans le disque / les charges dans la ram\n");
            kprint("1er param -> read or load\n");
            kprint("2eme param -> numero du disque (1 pour le slave et 0 pour le primaire)\n");
            kprint("3eme param -> adresse lba\n");
            kprint("4eme param -> quantite de secteur a lire ou charger\n");
        }else if(compare_word_buff("shutdown", argv[1])){
            kprint("eteint l'ordinateur\n");
            kprint("fonctionne sur VirtualBox et Qemu\n");
        }else{
            kprint("la commande n'existe pas\n");
        }
    }else{
        //detail de toutes les fonctions systèmes
        kprint("man -> donne les details des programmes\n");
        kprint("clear -> efface l'ecran\n");
        kprint("info -> infos systeme\n");
        kprint("echo -> affiche du texte\n");
        kprint("allocate -> alloue de la memoire\n");
        kprint("free -> libere de la memoire\n");
        kprint("dump -> affiche le contenu de la memoire\n");
        kprint("monitor -> affiche les ressources\n");
        kprint("load -> charge depuis le primary disque dans la heap\n");
        kprint("run -> lance un programme situe dans la ram\n");
        kprint("ata -> lit un disque ou charge un disque en memoire\n");
        kprint("shutdown -> arret de l'ordinateur\n");
    }
    
}