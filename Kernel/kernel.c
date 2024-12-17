
#define VIDEO_MEMORY  0xB8000

unsigned char inb(unsigned short port);
int is_key_ready();
void fprint(const char *str, const int y);
char *readKey();

void _start() {
    while (1) {
        char *key = readKey();
        fprint(key,0);
    }
}





