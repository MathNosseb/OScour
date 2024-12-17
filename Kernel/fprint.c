#define VIDEO_MEMORY 0xB8000
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

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
            *(char*)(addr + 1) = 0x07;  // Attribut (texte gris clair, fond noir)
            cursor_x++;
        }

        // Gestion du bout de ligne
        if (cursor_x >= SCREEN_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }

        // Évite de dépasser l'écran (pas de scroll)
        if (cursor_y >= SCREEN_HEIGHT) {
            cursor_y = SCREEN_HEIGHT - 1;
            cursor_x = 0;  // Écrit toujours sur la dernière ligne si l'écran est plein
        }
    }
}
