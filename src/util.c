#include <stdio.h>

char *strget(char *buffer, int max) {
    int size = 0;
    for (int i = 0; i < max - 1; i++) {
        char c;
        scanf("%c", &c);
        // detectó character inválido
        if (c == '\0') c = '?';
        if (c == '\n') break;
        buffer[i] = c;
        size++;
    }

    buffer[size] = '\0';
    return buffer;
}
