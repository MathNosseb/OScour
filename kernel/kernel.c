#include "output/vga.h"
#include "sys/struct.h"
#include "sys/string.h"
#include "sys/hex.h"
#include "input/keyboard.h"
#include "app/shell.h"
#include "mem/mem.h"
#include "sys/list.h"
#include "disque/disk.h"


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
    
    //on met l'adresse de la fonction vga_putchar dans la memoire
    //permet aux programmes externes de l utiliser
    void **table = (void **)0x504;
    *table = (void *)vga_putchar;
    //premier affichage
    vga_puchar_color("\nroot@local:", GREEN_ON_BLACK);

    //quantité totale de ram dans l appareil
    uint64_t total_mem = (get_total_ram()/1024/1024);    

    while (1) {
        //OUTPUT
        update_cursor();
        
        //les differents textes
        char heap_quantity[11];
        char stack_quantity[11];
        char total_ram[11];
        char total_heap[11];
        char vga_text[11];
        //les valeurs
        int heap_mem = get_heap_ram_usage();
        int stack_mem = get_stack_ram_usage();
        int heap_total_mem = get_heap_ram_usage_and_non_use();
        int vga_putchar_addr = *(int *)0x504;
        
        //mettre les valeurs dans les textes
        int_to_char(heap_mem, heap_quantity);//quantité de heap utilisé
        int_to_char(stack_mem, stack_quantity);//quantité de stack utilisé
        int_to_char(total_mem, total_ram);//quantité de ram dans l ordi
        int_to_char(heap_total_mem, total_heap);//distance depuis la derniere adresse de heap avec le start
        int_to_hex((uint32_t)vga_putchar_addr, vga_text);//adresse de la fonction print

        //affichage des valeurs dans un encadré en haut a droite
        print_at(54,0, "heap "); print_at(60,0,heap_quantity); print_at(64,0, " octets");
        print_at(54,1, "stack "); print_at(60,1,stack_quantity); print_at(64,1, " octets");
        print_at(54,2, "dispo "); print_at(60,2,total_ram); print_at(64,2, " Mo");
        print_at(53,3, "expend "); print_at(60,3,total_heap); print_at(64,3, " octets");
        print_at(54, 5, "print"); print_at(60, 5, vga_text); 


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
                    vga_putchar("\n");
                    //réaffichage de l utilisateur
                    vga_puchar_color("root@local:", GREEN_ON_BLACK);
                }
                //affichage du texte ecris
                vga_putchar(scanCodeToChar(scancode));
            }
            
        }        
    }
}