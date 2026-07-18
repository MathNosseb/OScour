#include "output/vga.h"
#include "sys/struct.h"
#include "sys/string.h"
#include "sys/hex.h"
#include "input/keyboard.h"
#include "app/shell.h"
#include "mem/mem.h"
#include "sys/list.h"
#include "disque/disk.h"

void put_fonctions();

void _start() {
    //affichage des infos de l OS
    load_art();
    //declaration des variables pour contenir les valeurs en texte
    char secteurs[1];
    char stack[7];
    //chargement depuis la stack -> ecris par le compilateur
    int nbr_secteur = *(int *)0x500; //nombre de secteurs chargé
    //calcul de la stack disponible connaissant stack size, kernel size et l emplacement
    uint32_t kernel_size = nbr_secteur * 512;
    uint32_t emplacement = kernel_size + 0x1000;
    uint32_t stack_size = 0x80000 - emplacement;
    //passage des nombres en texte
    int_to_char(nbr_secteur,secteurs);
    int_to_char(stack_size, stack);
    //affichage des secteurs chargés et de la quantité de stack dispo
    vga_puchar_color(secteurs, CYAN_ON_BLACK); vga_puchar_color(" secteurs, ", CYAN_ON_BLACK); 
    vga_puchar_color(stack, CYAN_ON_BLACK); vga_puchar_color(" Octets de stack dispo", CYAN_ON_BLACK); 
    
    put_fonctions();

    //premier affichage
    vga_puchar_color("\nroot@local:", GREEN_ON_BLACK);

    char **historic = allocate(10);//on sauvegarde les 10 dernieres commandes
    uint8_t historic_cursor = 0;

    while (1) {
        //OUTPUT
        update_cursor();
        
        //INPUT + COMMAND
        if (detect_buffer())
        {
            //recuperation du scan code
            uint8_t scancode = get_scancode();
            if (scancode != 0xFF)
            {   
                //si c'est une touche entrée
                if (scancode == 0x1C)
                {
                    //on detecte et execute la variable écrite
                    detect_command();
                    historic[historic_cursor] = allocate(strlen(get_command()));

                    strcpy(historic[historic_cursor], get_command());
                    if (historic_cursor < 10)
                        historic_cursor++;
                    //réaffichage de l utilisateur
                    vga_puchar_color("root@local:", GREEN_ON_BLACK);
                }
                //detection touche up
                if (scancode == 0x48)
                {
                    int pos = get_cursor();
                    if (historic_cursor > 0)
                    {
                        set_cursor(pos);
                        historic_cursor--;
                        clear_buffer(get_command(), 256);
                        strcpy(get_command(), historic[historic_cursor]);
                        vga_putchar(historic[historic_cursor]);
                    }
                    
                }
                //affichage du texte ecris
                vga_putchar(scanCodeToChar(scancode));
            }
            
        }        
    }
}

/// @brief met les adresses des fonctions systèmes en mémoire
void put_fonctions()
{
    //on met l'adresse de la fonction vga_putchar dans la memoire
    //permet aux programmes externes de l utiliser
    void **table = (void **)0x504;
    *table = (void *)vga_putchar;
}