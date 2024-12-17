#define STATUS_PORT   0x64
#define KEYBOARD_PORT 0x60

unsigned char inb(unsigned short port);
int is_key_ready() {
    return inb(STATUS_PORT) & 0x01;
}


char *readKey() {
    static char buffer[4];  // Static pour que la mémoire reste valide
    if (is_key_ready()) {
        unsigned char scancode = inb(KEYBOARD_PORT);

        // Conversion du scancode en chaîne hexadécimale
        buffer[0] = (scancode >> 4) < 10 ? ((scancode >> 4) + '0') : ((scancode >> 4) - 10 + 'A');
        buffer[1] = (scancode & 0x0F) < 10 ? ((scancode & 0x0F) + '0') : ((scancode & 0x0F) - 10 + 'A');
        buffer[2] = '\0';

        // Exemple : Si le scancode est 0x10, retourner "a"
        if (scancode == 0x10) {
            return "a";
        }
        if (scancode == 0x11) {
            return "z";
        }
        if (scancode == 0x12) {
            return "e";
        }
        if (scancode == 0x13) {
            return "r";
        }
        if (scancode == 0x14) {
            return "t";
        }
        if (scancode == 0x15) {
            return "y";
        }
        if (scancode == 0x16) {
            return "u";
        }
        if (scancode == 0x17) {
            return "i";
        }
        if (scancode == 0x18) {
            return "o";
        }
        if (scancode == 0x19) {
            return "p";
        }
        if (scancode == 0x1E) {
            return "q";
        }
        if (scancode == 0x1F) {
            return "s";
        }
        if (scancode == 0x20) {
            return "d";
        }
        if (scancode == 0x21) {
            return "f";
        }
        if (scancode == 0x22) {
            return "g";
        }
        if (scancode == 0x23) {
            return "h";
        }
        if (scancode == 0x24) {
            return "j";
        }
        if (scancode == 0x25) {
            return "k";
        }
        if (scancode == 0x26) {
            return "l";
        }
        if (scancode == 0x27) {
            return "m";
        }
        if (scancode == 0x2C) {
            return "w";
        }
        if (scancode == 0x2D) {
            return "x";
        }
        if (scancode == 0x2E) {
            return "c";
        }
        if (scancode == 0x2F) {
            return "v";
        }
        if (scancode == 0x30) {
            return "b";
        }
        if (scancode == 0x31) {
            return "n";
        }


        return "";  // Retourne la chaîne hexadécimale
    }
    return "";  // Retourne une chaîne vide si aucune touche n'est prête
}


