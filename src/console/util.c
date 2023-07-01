#include "util.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#include "../common/util.h"

#define TITULO_PAD TITULO_LEN / 2 - 2

void printTitulo() {
    const char *fila = strrepeat('#', TITULO_LEN);
    wprintf(L"%s\n", fila);
    wprintf(L"#%*sGO%*s#\n", TITULO_PAD, "", TITULO_PAD, "");
    wprintf(L"%s\n\n", fila);
}

void wprintCentro(const wchar_t *texto, int sizeLinea) {
    const int sizeTexto = wcslen(texto);
    if (sizeTexto > sizeLinea)
        wprintf(L"%ls\n", texto);
    else
        wprintf(L"%*s%ls\n", (sizeLinea - sizeTexto) / 2, "", texto);
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
                wprintf(L"\b");
                j--;
            }
            for (int k = j; k < i; k++)
                wprintf(L" ");
            wprintf(L"\n");
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
        wprintf(L"%s\n", texto);
    else
        wprintf(L"%*s%s\n", (sizeLinea - sizeTexto) / 2, "", texto);
}

char *strget(char *buffer, int max, char termino) {
    int size = 0;
    for (int i = 0; i < max - 1; i++) {
        char c;
        scanf("%c", &c);
        // detectó character inválido
        if (c == '\0') c = '?';
        if (c == '\n' || c == termino) break;
        buffer[i] = c;
        size++;
    }

    buffer[size] = '\0';
    if (strlen(buffer) == max - 1) getchar();
    return buffer;
}

bool getInt(int *n) {
    char buffer[INT_STR_MAX];
    strget(buffer, INT_STR_MAX, ' ');
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

bool confirmar(const wchar_t *action, bool predeterminado) {
    if (predeterminado)
        wprintf(L"¿Estás segur@ que quieres %ls? (y/n): ", action);
    else
        wprintf(L"%ls (y/n): ", action);

    char confirmado;
    scanf("%c", &confirmado);
    while (confirmado != 'y' && confirmado != 'n' && confirmado != 'Y' && confirmado != 'N') {
        wprintf(L"Opción inválida. Ingresa \"y\" o \"n\": ");
        scanf("%c", &confirmado);
    }
    getchar();
    return confirmado == 'y' || confirmado == 'Y';
}

void esperar(unsigned int segundos) {
    fflush(stdout);
    sleep(segundos);
}
