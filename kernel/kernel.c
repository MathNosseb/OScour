#define VIDEO_MEMORY 0xB8000
#define WHITE_ON_BLACK 0x07


typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;

int counter = 0;
void vga_putchar(char*);
static inline void outb(uint16_t, uint8_t);
static inline uint8_t inb(uint16_t);
char* scanCodeToChar(uint8_t);

void _start() {
    vga_putchar("\nBoot Done");

    while (1) {
        counter = (counter < 0) ? 0 : counter % (25*80);
        uint8_t status = inb(0x64);
        if (!(status & 0x01)) continue;  // buffer vide, rien à lire

        uint8_t scanCode = inb(0x60);
        uint8_t bit7     = (scanCode >> 7) & 1; //1 = relâché, 0 = pressé
        uint8_t key      = scanCode & 0x7F; //on sup le 7 eme bit pour avoir le scanCode sans info

        if (bit7)
        {
            vga_putchar(scanCodeToChar(key));
        }
        //asm volatile("hlt");

        outb(0x3D4, 0x0F);
        outb(0x3D5, (uint8_t)(counter & 0xFF));

        outb(0x3D4, 0x0E);
        outb(0x3D5, (uint8_t)(counter >> 8));
    }
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

    case 0x0E: key = "\b"; break;//backspace
    case 0x39: key = " "; break;//space
    case 0x1C: key = "\n"; break;//enter
    case 0x0F: key = "\t"; break;//tab

    case 0x48: counter -= 80; break;//up
    case 0x50: counter += 80; break;//down
    case 0x4D: counter += 1; break;//right
    case 0x4B: counter -= 1; break;//left

    default: break;
    }
    return key;
}

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
    /* There's an outb %al, $imm8 encoding, for compile-time constant port numbers that fit in 8b. (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}


void vga_putchar(char *texte)
{
    volatile uint16_t* p;
    p = (volatile uint16_t *)VIDEO_MEMORY;

    int index = 0;
    while (texte[index] != '\0')
    {
        int special = 0;
        if (texte[index] == '\n')
        {
           special = 1;
            counter = (counter / 80 + 1) * 80;  // pas de -= 1
            index++;
            
        }
        if (texte[index] == '\b')
        {
            //backspace
            special=1;
            counter-=1;
            index+=1;
            vga_putchar(" ");
            counter-=1;
            continue;
        }
        if (texte[index] == '\t')
        {
            //tabulation
            special=1;
            counter += 3;
            index++;
            break;
            
        }
        if (special==0)
        {
            p[counter] = (WHITE_ON_BLACK << 8) | texte[index];
            counter+=1;
            index+=1;
        }
        
    }
    
    
}


