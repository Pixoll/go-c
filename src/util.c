#include "util.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// signo + 10 dígitos + nulo
#define INT_STR_MAX 12
#define TITULO_PAD TITULO_LEN / 2 - 2

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

/* No modifica el buffer original! */
char *strpadleft(char *buffer, int max, char fill) {
    const int size = strlen(buffer);
    if (size >= max) return buffer;
    char *resultado = malloc((max + 1) * sizeof(char));
    resultado[0] = '\0';
    strcat(resultado, strrepeat(fill, max - size));
    strcat(resultado, buffer);
    resultado[max] = '\0';
    return resultado;
}

/* No modifica el buffer original! */
char *strpadright(char *buffer, int max, char fill) {
    const int size = strlen(buffer);
    if (size >= max) return buffer;
    char *resultado = malloc((max + 1) * sizeof(char));
    resultado[0] = '\0';
    strcat(resultado, buffer);
    strcat(resultado, strrepeat(fill, max - size));
    resultado[max] = '\0';
    return resultado;
}

wchar_t *wcsrepeat(wchar_t fill, int size) {
    wchar_t *buffer = malloc(sizeof(wchar_t) * (size + 1));
    for (int i = 0; i < size; i++)
        buffer[i] = fill;
    buffer[size] = '\0';
    return buffer;
}

/* No modifica el buffer original! */
wchar_t *wcspadright(wchar_t *buffer, int max, wchar_t fill) {
    const int size = wcslen(buffer);
    if (size >= max) return buffer;
    wchar_t *resultado = malloc((max + 1) * sizeof(wchar_t));
    resultado[0] = '\0';
    wcscat(resultado, buffer);
    wcscat(resultado, wcsrepeat(fill, max - size));
    resultado[max] = '\0';
    return resultado;
}

wchar_t *strtowcs(char *buffer) {
    const int size = strlen(buffer);
    wchar_t *transformado = malloc((size + 1) * sizeof(wchar_t));
    for (int i = 0; i < size; i++)
        transformado[i] = buffer[i];
    transformado[size] = '\0';
    return transformado;
}

void printTitulo() {
    const char *fila = strrepeat('#', TITULO_LEN);
    const char *espacio = strrepeat(' ', TITULO_PAD);
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

char *intATexto(int n) {
    char *buffer = malloc(INT_STR_MAX * sizeof(char));
    itoa(n, buffer, 10);
    buffer = realloc(buffer, strlen(buffer) + 1);
    return buffer;
}

int intDigits(int n) {
    char *buffer = malloc(INT_STR_MAX * sizeof(char));
    itoa(n, buffer, 10);
    const int digits = strlen(buffer);
    free(buffer);
    return digits;
}

void limpiarConsola() {
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif

#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}

unsigned long now() {
    return time(0);
}
