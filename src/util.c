#include "util.h"

#include <stdbool.h>
// Para Linux
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// signo + 10 dígitos + nulo
#define INT_STR_MAX 12
#define TITULO_PAD TITULO_LEN / 2 - 2
#define YEAR_0 1900

const char meses[12][4] = {
    "Ene",
    "Feb",
    "Mar",
    "Abr",
    "May",
    "Jun",
    "Jul",
    "Ago",
    "Sep",
    "Oct",
    "Nov",
    "Dic",
};

char *strrepeat(char fill, int size) {
    char *buffer = malloc(size + 1);
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

char *strpadleft(const char *buffer, int max, char fill) {
    const int size = strlen(buffer);
    if (size >= max) return (char *)buffer;
    char *resultado = malloc(max + 1);
    resultado[0] = '\0';
    strcat_s(resultado, max + 1, strrepeat(fill, max - size));
    strcat_s(resultado, max + 1, buffer);
    resultado[max] = '\0';
    return resultado;
}

char *strpadright(const char *buffer, int max, char fill) {
    const int size = strlen(buffer);
    if (size >= max) return (char *)buffer;
    char *resultado = malloc(max + 1);
    resultado[0] = '\0';
    strcat_s(resultado, max + 1, buffer);
    strcat_s(resultado, max + 1, strrepeat(fill, max - size));
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

wchar_t *wcspadright(const wchar_t *buffer, int max, wchar_t fill) {
    const int size = wcslen(buffer);
    if (size >= max) return (wchar_t *)buffer;
    wchar_t *resultado = malloc((max + 1) * sizeof(wchar_t));
    resultado[0] = '\0';
    wcscat(resultado, buffer);
    wcscat(resultado, wcsrepeat(fill, max - size));
    resultado[max] = '\0';
    return resultado;
}

wchar_t *strtowcs(const char *buffer) {
    const int size = strlen(buffer);
    wchar_t *transformado = malloc((size + 1) * sizeof(wchar_t));
    int nuevoSize = 0;
    for (int i = 0; i < size; i++) {
        const char c = buffer[i];
        if (c == '\b') break;
        if (c >= 0) {
            transformado[nuevoSize++] = c;
            continue;
        }

        const unsigned char next = buffer[++i];
        const wchar_t real = next == 166 ? next : next + 0100;
        transformado[nuevoSize++] = real;
    }

    transformado[nuevoSize] = '\0';
    return transformado;
}

void printTitulo() {
    const char *fila = strrepeat('#', TITULO_LEN);
    const char *espacio = strrepeat(' ', TITULO_PAD);
    printf("%s\n", fila);
    printf("#%sGO%s#\n", espacio, espacio);
    printf("%s\n\n", fila);
}

void wprintCentro(const wchar_t *texto, int sizeLinea) {
    const int sizeTexto = wcslen(texto);
    if (sizeTexto > sizeLinea)
        wprintf(L"%ls\n", texto);
    else
        wprintf(L"%s%ls\n", strrepeat(' ', (sizeLinea - sizeTexto) / 2), texto);
}

void wprintConLineLimit(const wchar_t *texto, int limit) {
    int lineaSize = 0;
    for (int i = 0; i <= wcslen(texto); i++) {
        const wchar_t wc = texto[i];
        if (wc == '\n') lineaSize = 0;
        if (lineaSize != 0 && lineaSize % limit == 0) {
            int j = i;
            while (j >= 0) {
                if (texto[j - 1] == ' ') break;
                printf("\b");
                j--;
            }
            for (int k = j; k < i; k++)
                printf(" ");
            printf("\n");
            for (int k = j; k < i; k++) {
                wprintf(L"%lc", texto[k]);
                lineaSize++;
            }
        }
        wprintf(L"%lc", wc);
        lineaSize++;
    }
}

void printCentro(const char *texto, int sizeLinea) {
    const int sizeTexto = strlen(texto);
    if (sizeTexto > sizeLinea)
        printf("%s\n", texto);
    else
        printf("%s%s\n", strrepeat(' ', (sizeLinea - sizeTexto) / 2), texto);
}

char *strget(char *buffer, int max) {
    int size = 0;
    for (int i = 0; i < max - 1; i++) {
        char c;
        scanf_s("%c", &c);
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
    char *buffer = malloc(INT_STR_MAX);
    itoa(n, buffer, 10);
    buffer = realloc(buffer, strlen(buffer) + 1);
    return buffer;
}

int intDigits(int n) {
    char *buffer = malloc(INT_STR_MAX);
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

// Para no tener que pedir <time.h>
long long now() {
    return time(0);
}

// Si ms 0 devuelve fecha actual
char *obtenerFecha(long long ms) {
    const time_t tms = ms == 0 ? now() : ms;
    struct tm *tiempo = localtime(&tms);
    char *fecha = malloc(12);
    sprintf_s(fecha, 12, "%d %s %d", tiempo->tm_mday, meses[tiempo->tm_mon], YEAR_0 + tiempo->tm_year);
    return fecha;
}

bool confirmar(const wchar_t *action) {
    wprintf(L"¿Estás segur@ que quieres %ls? (y/n): ", action);
    char confirmado;
    scanf_s("%c", &confirmado);
    while (confirmado != 'y' && confirmado != 'n' && confirmado != 'Y' && confirmado != 'N') {
        wprintf(L"Opción inválida. Ingresa \"y\" o \"n\": ");
        scanf_s("%c", &confirmado);
    }
    getchar();
    return confirmado == 'y' || confirmado == 'Y';
}
