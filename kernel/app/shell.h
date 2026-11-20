#pragma once
#include "../output/vga.h"
#include "../sys/struct.h"
#include "../sys/string.h"
#include "../input/keyboard.h"
#include "../mem/mem.h"
#include "../mem/e820.h"
#include "../sys/hex.h"
#include "../sys/struct.h"
#include "../disque/disk.h"
#include "man.h"
#include "programs.h"
#include "banner.h"

void            shell_loop(void);
static void     detect_command(char *command);