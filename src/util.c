#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_STR_MAX 12
#define TITULO_LEN 50

char *strrepeat(char fill, int size) {
    char *buffer = malloc(sizeof(char) * (size + 1));
    for (int i = 0; i < size; i++)
        buffer[i] = fill;
    buffer[size] = '\0';
    return buffer;
}

char *strempty(char *buffer) {
    const int size = strlen(buffer);
    for (int i = 0; i < size; i++)
        buffer[i] = '\0';
    return buffer;
}

void printTitulo() {
    const char *fila = strrepeat('#', TITULO_LEN);
    const char *espacio = strrepeat(' ', 23);
    printf("%s\n", fila);
    printf("#%sGO%s#\n", espacio, espacio);
    printf("%s\n\n", fila);
}

void wprintCentro(wchar_t *texto, int sizeLinea) {
    const int sizeTexto = wcslen(texto);
    if (sizeTexto > sizeLinea) {
        wprintf(L"%ls\n", texto);
        return;
    }

    const char *espacio = strrepeat(' ', (sizeLinea - sizeTexto) / 2);
    wprintf(L"%s%ls\n", espacio, texto);
}

void printCentro(char *texto, int sizeLinea) {
    const int sizeTexto = strlen(texto);
    if (sizeTexto > sizeLinea) {
        printf("%s\n", texto);
        return;
    }

    const char *espacio = strrepeat(' ', (sizeLinea - sizeTexto) / 2);
    printf("%s%s\n", espacio, texto);
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
