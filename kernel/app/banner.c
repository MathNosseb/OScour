#include "banner.h"

void    load_art(void)
{
	kprint("   ____   _____                             ___  \n");
	kprint("  / __ \\ / ____|                           |__ \\ \n");
	kprint(" | |  | | (___   ___ ___  _   _ _ __  __   __ ) |\n");
	kprint(" | |  | |\\___ \\ / __/ _ \\| | | | '__| \\ \\ / // / \n");
	kprint(" | |__| |____) | (_| (_) | |_| | |     \\ V // /_ \n");
	kprint("  \\____/|_____/ \\___\\___/ \\__,_|_|      \\_/|____|\n");
	kprint_color("Oscour made by mathnosseb v2.1.2\n", CYAN_ON_BLACK);
}

void	kernel_panic_art(void)
{
	kprint_color("\n\t\t\t\t\t       .--------------.       \n", RED_ON_BLACK);
	kprint_color("\t\t\t\t\t     .'   .--.  .--.   '.     \n", RED_ON_BLACK);
	kprint_color("\t\t\t\t\t    /    / () \\/ () \\    \\    \n", RED_ON_BLACK);
	kprint_color("\t\t\t\t\t   |     \\    /\\    /     |   \n", RED_ON_BLACK);
	kprint_color("\t\t\t\t\t   |      '--'  '--'      |   \n", RED_ON_BLACK);
	kprint_color("\t\t\t\t\t   |          /\\          |   \n", RED_ON_BLACK);
	kprint_color("\t\t\t\t\t   |         '--'         |   \n", RED_ON_BLACK);
	kprint_color("\t\t\t\t\t   |       .------.       |   \n", RED_ON_BLACK);
	kprint_color("\t\t\t\t\t    \\     / /\\/\\/\\ \\     /    \n", RED_ON_BLACK);
	kprint_color("\t\t\t\t\t     '.  |  \\/\\/\\/  |  .'     \n", RED_ON_BLACK);
	kprint_color("\t\t\t\t\t       '-.\\        /.-'       \n", RED_ON_BLACK);
	kprint_color("\t\t\t\t\t           '------'           \n", RED_ON_BLACK);
}