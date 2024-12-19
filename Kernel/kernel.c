
void fprint(const char *str);
void initialiationVGA();
char *readKey();

void _start() {
    //initialiationVGA();
    while (1) {
        char *key = readKey();
        fprint(key);
    }
}
