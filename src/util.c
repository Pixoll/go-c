#include "util.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

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

wchar_t *wcslower(const wchar_t *buffer) {
    const int size = wcslen(buffer);
    wchar_t *resultado = malloc((size + 1) * sizeof(wchar_t));
    for (int i = 0; i < size; i++) {
        const wchar_t c = buffer[i];
        if (c < 'A' || c > 'Z')
            resultado[i] = c;
        else
            resultado[i] = c + ('a' - 'A');
    }
    resultado[size] = '\0';
    return resultado;
}

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

int intDigits(int n) {
    char *buffer = malloc(INT_STR_MAX);
    snprintf(buffer, INT_STR_MAX, "%d", n);
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
    snprintf(fecha, 12, "%d %s %d", tiempo->tm_mday, meses[tiempo->tm_mon], YEAR_0 + tiempo->tm_year);
    return fecha;
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

void swap(int *px, int *py) {
    const int temp = *px;
    *px = *py;
    *py = temp;
}
