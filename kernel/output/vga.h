#include "../sys/struct.h"
#define VIDEO_MEMORY     0xB8000
#define WHITE_ON_BLACK   0x07
#define GREEN_ON_BLACK   0x02
#define CYAN_ON_BLACK    0x03
#define RED_ON_BLACK     0x04
#define YELLOW_ON_BLACK  0x0E
#define WHITE_ON_BLUE    0x17
#define BLACK_ON_WHITE   0x70


void vga_putchar(char *);
void vga_puchar_color(char *, uint8_t);
void update_cursor();
void set_cursor(int);
int get_cursor();
void clear_screen();
