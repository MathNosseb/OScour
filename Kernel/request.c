unsigned char inb(unsigned short port) {
    unsigned char value;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}