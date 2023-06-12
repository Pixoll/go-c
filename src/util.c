#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define INT_STR_MAX 12

void printTitulo() {
    printf("##################################\n");
    printf("#               GO               #\n");
    printf("##################################\n");
    printf("\n");
}

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

bool getInt(int *n) {
    char buffer[INT_STR_MAX];
    strget(buffer, INT_STR_MAX);
    for (int i = 0; i < INT_STR_MAX; i++) {
        const char c = buffer[i];
        if (c == '\0') break;
        if (c >= '0' && c <= '9') continue;
        if (c == '-' && i == 0) continue;
        return false;
    }
    *n = atoi(buffer);
    return true;
}

void limpiarConsola() {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif

#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}
