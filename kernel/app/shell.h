#pragma once
#include "../output/vga.h"
#include "../sys/struct.h"
#include "../sys/string.h"
#include "../input/keyboard.h"

void    shell_loop(void);
static void    detect_command(char *command);