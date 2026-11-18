#include "keyboard.h"
#include "../sys/struct.h"
#include "../output/vga.h"

int detect_buffer()
{
    uint8_t status = inb(0x64);
    if (!(status & 0x01)) return 0;// buffer vide, rien à lire
    return 1;
}

uint8_t get_scancode()
{
    uint8_t scanCode = inb(0x60);
    uint8_t key      = scanCode & 0x7F; //on sup le 7 eme bit pour avoir le scanCode sans info
    uint8_t bit7     = (scanCode >> 7) & 1; //1 = relâché, 0 = pressé
    if (bit7) return key;
    return 0xFF;
}

char    scanCodeToChar(uint8_t scanCode)
{
    switch (scanCode)
    {
        case 0x10: return 'q';
        case 0x11: return 'w';
        case 0x12: return 'e';
        case 0x13: return 'r';
        case 0x14: return 't';
        case 0x15: return 'y';
        case 0x16: return 'u';
        case 0x17: return 'i';
        case 0x18: return 'o';
        case 0x19: return 'p';
        case 0x1E: return 'a';
        case 0x1F: return 's';
        case 0x20: return 'd';
        case 0x21: return 'f';
        case 0x22: return 'g';
        case 0x23: return 'h';
        case 0x24: return 'j';
        case 0x25: return 'k';
        case 0x26: return 'l';
        case 0x32: return 'm';
        case 0x2C: return 'z';
        case 0x2D: return 'x';
        case 0x2E: return 'c';
        case 0x2F: return 'v';
        case 0x30: return 'b';
        case 0x31: return 'n';
        case 0x02: return '1';
        case 0x03: return '2';
        case 0x04: return '3';
        case 0x05: return '4';
        case 0x06: return '5';
        case 0x07: return '6';
        case 0x08: return '7';
        case 0x09: return '8';
        case 0x0A: return '9';
        case 0x0B: return '0';
        case SC_SPACE:     return ' ';
        case SC_TAB:       return '\t';
        case SC_ENTER:     return '\n';
        case SC_BACKSPACE: return '\b';
        default:           return 0;
    }
}