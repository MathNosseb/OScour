#pragma once
#include "../sys/struct.h"
#include "../mem/mem.h"
#include "../sys/string.h"
#define VIDEO_MEMORY     0xB8000
#define WHITE_ON_BLACK   0x0F
#define GREEN_ON_BLACK   0x02
#define CYAN_ON_BLACK    0x03
#define RED_ON_BLACK     0x04
#define YELLOW_ON_BLACK  0x0E
#define WHITE_ON_BLUE    0x17
#define BLACK_ON_WHITE   0x70
#define VGA_COLS      80
#define VGA_ROWS      25
#define VGA_CRTC_ADDR 0x3D4
#define VGA_CRTC_DATA 0x3D5
#define TAB_WIDTH     4

void    kprint(char *);
void    write(char *s1, int len);
void    kprint_color(char *, uint8_t);
void    update_cursor(void);
void    set_cursor(int);
int     get_cursor(void);
void    clear_screen(void);
void    print_at(int x, int y, char *text);
void    scroll_screen();
void    kput_number(int nb);