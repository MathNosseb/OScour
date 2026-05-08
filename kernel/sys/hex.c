#include "hex.h"

int hex_to_int(char *s) {
    int result = 0;

    while (*s) {
        result *= 16;

        if (*s >= '0' && *s <= '9')
            result += *s - '0';
        else if (*s >= 'a' && *s <= 'f')
            result += *s - 'a' + 10;
        else if (*s >= 'A' && *s <= 'F')
            result += *s - 'A' + 10;

        s++;
    }

    return result;
}

void int_to_hex(int n, char *out) {
    char hex[] = "0123456789ABCDEF";
    char tmp[16];
    int i = 0;

    if (n == 0) {
        out[0] = '0';
        out[1] = '\0';
        return;
    }

    while (n > 0) {
        tmp[i++] = hex[n % 16];
        n /= 16;
    }

    int j = 0;
    while (i > 0) {
        out[j++] = tmp[--i];
    }

    out[j] = '\0';
}