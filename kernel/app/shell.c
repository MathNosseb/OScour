#include "shell.h"

void    shell_loop(void)
{
	uint8_t scancode;
	uint8_t command_length;
	char    key;
	char    *command;

	command_length = 0;
	kprint_color("\nroot@local:", GREEN_ON_BLACK);
	command = allocate(256);
	memset(command, 0, 256);
	while (1)
	{
		update_cursor();
		if (detect_buffer())
		{
			scancode = get_scancode();
			if (scancode != 0xFF)
			{
				key = scanCodeToChar(scancode);
				//si c'est une touche entrée
				if (key == '\n')
				{
					command[command_length] = '\0';
					detect_command(command);
					command_length = 0;
					memset(command, 0, 256);
					kprint_color("root@local:", GREEN_ON_BLACK);
				}
				else if (key == '\b')
				{
					if (command_length > 0)
					{
						command_length--;
						write(&key, 1);
					}
				}
				else if (key && command_length < 255)
				{
					command[command_length++] = key;
					write(&key, 1);
				}
			}
		}        
	}
}

static void    detect_command(char *command)
{
	int     argc;
	char    **argv;

	//commandes qui ont besoin de ne pas avoir de
	//mouvement de memoire pour avoir le bon resultat
	if (strcmp("dump", command) == 0)
	{        
		dump_heap();
		kprint("\n");
		return;
	}
	argv = split_words(command, &argc);
	if (argc == 0)
		kprint("\n");
	else if (strcmp("man", argv[0]) == 0)
	{
		kprint("\n");
		system_functions_descriptions(argc, argv);
	}
	else if (strcmp("shutdown", argv[0]) == 0)
	{
		outw(0x604, 0x2000);//qemu
		outw(0x4004, 0x3400);//virtualbox
		while (1)
			__asm__ volatile ("hlt");
	}
	else if (strcmp("clear", argv[0]) == 0)
		clear_screen();
	else if (strcmp("info", argv[0]) == 0)
	{
		kprint("\n");
		load_art();
	}
	else if (strcmp("echo", argv[0]) == 0)
	{
		kprint("\n");
		for (int index = 0; index < argc - 1; index++)
		{
			kprint(argv[index + 1]);
			kprint(" ");
		}
	}
	else if (strcmp("allocate", argv[0]) == 0)
	{
		if (argc < 2)
		{
			free(argv); 
			return;
		}
		//on aloue la ram a la valeur de argv[1]
		int ram = atoi(argv[1]);
		allocate(ram);
		kprint("\ndone");
	}
	else if (strcmp("free", argv[0]) == 0)
	{
		if (argc < 2)
		{
			free(argv); 
			return;
		} 
		free((uint32_t *)hex_to_int(argv[1]));
	}

	else if (strcmp("monitor", argv[0]) == 0)
	{
		//affiche les ressources de l'ordinateur
		//les ressources
		uint32_t total_mem = (get_total_ram_size() / 1024);
		uint32_t heap_mem = get_heap_ram_usage();
		uint32_t stack_mem = get_stack_usage();
		uint32_t heap_total_mem = get_heap_ram_usage_and_non_use();
		uint32_t tot = get_mem_usage() / 1024;

		//print
		kprint("\n");
		kprint("ressources ram: "); kput_number(total_mem);kprint(" Ko");kprint("\n");
		kprint("heap usage: "); kput_number(heap_mem);kprint(" Octets");kprint(" / ");kput_number(heap_max); kprint(" Octets\n");
		kprint("stack usage: "); kput_number(stack_mem);kprint(" Octets");kprint(" / ");kput_number(get_stack_size() / 1024);kprint(" Ko\n");
		kprint("heap deployment: "); kput_number(heap_total_mem);kprint(" Octets");kprint(" / ");kput_number(heap_max); kprint(" Octets\n");
		kput_number(tot); kprint("Ko / "); kput_number(total_mem); kprint("Ko");
	}
	else if (strcmp("run", argv[0]) == 0)
	{
		if (argc < 2)
		{
			free(argv); 
			return;
		} 
		//recuperer l adresse
		kprint("\n");
		run_program((uint32_t *)hex_to_int(argv[1]));
	}
	else if (strcmp("ata", argv[0]) == 0)
	{
		if (argc < 5)
		{            
			free(argv); 
			return;
		} 

		kprint("\n");
		int disk_num = atoi(argv[2]);
		uint32_t lba = atoi(argv[3]);
		int secteur_num = atoi(argv[4]);

		if(strcmp("read", argv[1]) == 0)
			read_ata(disk_num, lba, secteur_num);
		else if (strcmp("load", argv[1]) == 0)
			load_ata(disk_num, lba, secteur_num);
		else if (strcmp("write", argv[1]) == 0)
		{
			//le premier jeu de données est a argv[4]
			//donc les data c est argc - 4
			int	i;
			i = 4;
			while (i < argc - 4)
			{
				kprint(argv[i]);
				if (hex_to_int(argv[i]) == 255)
					kprint("tets");
				kput_number(hex_to_int(argv[i++]));
			}
		}           
	}
	else
		kprint_color("\ncommande non reconnu", RED_ON_BLACK);
	kprint("\n");
	free(argv);
}
