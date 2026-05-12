#include "keyboard.h"
#include "../sys/struct.h"
#include "../output/vga.h"

char lineBuffer[256];
uint8_t line_len = 0;

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

void clear_buffer(char *buffer, int size)
{
    for (int i = 0; i < size; i++) buffer[i] = 0;
}

char* scanCodeToChar(uint8_t scanCode)
{
    char *key = "";
    switch (scanCode)
    {
    case 0x10: key = "a"; break;//A
    case 0x11: key = "z"; break;//Z
    case 0x12: key = "e"; break;//e
    case 0x13: key = "r"; break;//r
    case 0x14: key = "t"; break;//t
    case 0x15: key = "y"; break;//y
    case 0x16: key = "u"; break;//u
    case 0x17: key = "i"; break;//i
    case 0x18: key = "o"; break;//o
    case 0x19: key = "p"; break;//p
    case 0x20: key = "d"; break;//d
    case 0x21: key = "f"; break;//f
    case 0x22: key = "g"; break;//g
    case 0x23: key = "h"; break;//h
    case 0x24: key = "j"; break;//j
    case 0x25: key = "k"; break;//k
    case 0x26: key = "l"; break;//l
    case 0x27: key = "m"; break;//f
    case 0x1E: key = "q"; break;//a
    case 0x1F: key = "s"; break;//s
    case 0x2C: key = "w"; break;//w
    case 0x2D: key = "x"; break;//x
    case 0x2E: key = "c"; break;//c
    case 0x2F: key = "v"; break;//v
    case 0x30: key = "b"; break;//b
    case 0x31: key = "n"; break;//n

    case 0x02: key = "1"; break;
    case 0x03: key = "2"; break;
    case 0x04: key = "3"; break;
    case 0x05: key = "4"; break;
    case 0x06: key = "5"; break;
    case 0x07: key = "6"; break;
    case 0x08: key = "7"; break;
    case 0x09: key = "8"; break;
    case 0x0A: key = "9"; break;
    case 0x0B: key = "0"; break;

    case 0x0E:
        if (line_len > 0)
        {
            line_len--;
            lineBuffer[line_len] = '\0';
        }
        return "\b";
    case 0x39: key = " "; break;//space
    case 0x1C: line_len = 0; clear_buffer(lineBuffer, 256); return "" ;//enter
    case 0x0F: key = "\t"; break;//tab

    case 0x48: set_cursor(get_cursor()-80); break;//up
    case 0x50: set_cursor(get_cursor()+80); break;//down
    case 0x4D: set_cursor(get_cursor()+1); break;//right
    case 0x4B: set_cursor(get_cursor()-1); break;//left

    default: break;
    }
    lineBuffer[line_len] = *key;
    line_len++;
    return key;
}


char *get_command()
{
    return lineBuffer;
}