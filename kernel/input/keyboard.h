#include "../sys/struct.h"

int detect_buffer();
uint8_t key_pressed();
uint8_t get_scancode();
char *scanCodeToChar(uint8_t);
char *get_command();
void clear_buffer(char *, int);