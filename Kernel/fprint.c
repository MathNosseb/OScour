void fprint(const char *str) {
    int y = 0;
    int x = 0;
    int index = y * 80 + x;
    int addrVideo = 0xB8000 + (index*2);

    for (int i = 0; str[i] != '\0'; i++) {
        *(char*)addrVideo = str[i];
        x++;
        index = y * 80 + x;
        addrVideo = 0xB8000 + (index*2);
    }

}