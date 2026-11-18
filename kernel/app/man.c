#include "man.h"

/// @brief detail les commandes -> commande man
/// @param argv la commande split en liste de mots
void    system_functions_descriptions(int argc, char **argv)
{
    if (argc >= 2)
    {
        //demande le detail d'une fonction
        if (strcmp("man", argv[1]) == 0)
        {
            kprint("INCEPTION\n");
            kprint("affiche les details d'une fonction systeme\n");
        }
        else if (strcmp("clear", argv[1]) == 0)
        {
            kprint("efface l'ecran\n");
        }else if(strcmp("info", argv[1]) == 0)
        {
            kprint("affiche les infos systeme\n");
        }else if(strcmp("echo", argv[1]) == 0)
        {
            kprint("affiche du texte a l'ecran\n");
            kprint("les parametres sont tous affiches\n");
        }else if(strcmp("allocate", argv[1]) == 0)
        {
            kprint("alloue une certaine quantite de memoire en octet dans la heap\n");
            kprint("prend un parametre qui est un nombre en octet\n");
            kprint("affiche \"done\" quand il a fini\n");
        }else if(strcmp("free", argv[1]) == 0)
        {
            kprint("libere la ram alloue avec allocate\n");
            kprint("prend l adresse de la donnee\n");
        }else if(strcmp("dump", argv[1]) == 0)
        {
            kprint("affiche les blocs memoire alloue libre ou non\n");
            kprint("permet de connaitre l'adresse des blocs\n");
        }else if(strcmp("monitor", argv[1]) == 0)
        {
            kprint("affiche les ressources de l'ordinateur\n");
        }else if(strcmp("load", argv[1]) == 0)
        {
            kprint("charge depuis le primary disque dans la heap\n");
            kprint("1er parametre: adresse sur le disque\n");
            kprint("2eme parametre: taille de la donnee en octet\n");
            kprint("affiche l'adresse dansizes la heap du debut de la donnee\n");
            kprint("affiche en hexadecimal les donnees stockes\n");
        }else if(strcmp("run", argv[1]) == 0)
        {
            kprint("lance un programme qui se trouve dans la heap\n");
            kprint("1er parametre: adresse du programme dans la heap\n");
            kprint("saute au programme comme une fonction standart\n");
        }else if(strcmp("ata", argv[1]) == 0)
        {
            kprint("affiche des donnees dans le disque / les charges dans la ram\n");
            kprint("1er param -> read or load\n");
            kprint("2eme param -> numero du disque (1 pour le slave et 0 pour le primaire)\n");
            kprint("3eme param -> adresse lba\n");
            kprint("4eme param -> quantite de secteur a lire ou charger\n");
        }else if(strcmp("shutdown", argv[1]) == 0){
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