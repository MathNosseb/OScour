#define VIDEO_MEMORY 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25


//petit outb pour envoyer la position du curseur
void outb(unsigned short port, unsigned char value);


int cursor_x = 0;  // Position actuelle de la colonne
int cursor_y = 0;  // Position actuelle de la ligne



void fprint(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {  // Gère un retour à la ligne
            cursor_x = 0;
            cursor_y++;
        } else {
            int addr = VIDEO_MEMORY + ((cursor_y * SCREEN_WIDTH + cursor_x) * 2);
            *(char*)addr = str[i];
            *(char*)(addr + 1) = 0x02;  // Attribut (texte gris clair, fond noir)
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
