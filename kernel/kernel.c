#define VIDEO_MEMORY 0xB8000
#define WHITE_ON_BLACK 0x07

void print(const char* str) {
    volatile unsigned char* vga = (unsigned char*)VIDEO_MEMORY;
    int i = 0;
    while (str[i] != '\0') {
        vga[i * 2]     = str[i];          // caractère
        vga[i * 2 + 1] = WHITE_ON_BLACK;  // couleur
        i++;
    }
}



void _start() {
    print("Hello, World! test test test tes test test");
    while (1) {
        asm volatile("hlt"); // met le CPU en pause
    }
}
