#define VIDEO_MEMORY 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define BYTES_PER_CHAR 2

void outb(unsigned short port, unsigned char value);

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

int cursor_x = 0;  // Position actuelle de la colonne
int cursor_y = 0;  // Position actuelle de la ligne


#define LINE_LENGTH 80
#define NUM_LINES 25


void fprint(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {  // Gère un retour à la ligne
            cursor_x = 0;
            cursor_y++;
        } else {
            int addr = VIDEO_MEMORY + ((cursor_y * SCREEN_WIDTH + cursor_x) * 2);
            *(char*)addr = str[i];
            *(char*)(addr + 1) = 0x07;  // Attribut (texte gris clair, fond noir)
            cursor_x++;
        }

        // Gestion du bout de ligne
        if (cursor_x >= SCREEN_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }

        //mouvement du curseur
        unsigned short position = cursor_y * SCREEN_WIDTH + cursor_x;
        outb(0x3D4, 0x0F);
        outb(0x3D5, (unsigned char)(position & 0xFF));
        outb(0x3D4, 0x0E); // Sélectionner l'octet de poids fort
        outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));

    }

}
