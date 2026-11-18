#include "../sys/struct.h"

#define SC_BACKSPACE 0x0E
#define SC_TAB       0x0F
#define SC_ENTER     0x1C
#define SC_SPACE     0x39

int     detect_buffer();
uint8_t get_scancode();
char    scanCodeToChar(uint8_t);