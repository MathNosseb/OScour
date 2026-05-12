#include "../output/vga.h"
#include "../sys/struct.h"
#include "../sys/string.h"
#include "../input/keyboard.h"

void detect_command();
int compare_word_buff(char *, char *);
int compare_first_word_buffer(char *, char *);
char *del_prefix(char *);
