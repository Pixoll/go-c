#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>

#include "datos.h"

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

int intDigits(int n) {
    char *buffer = malloc(INT_STR_MAX);
    snprintf(buffer, INT_STR_MAX, "%d", n);
    const int digits = strlen(buffer);
    free(buffer);
    return digits;
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

void swap(int *px, int *py) {
    const int temp = *px;
    *px = *py;
    *py = temp;
}

// parámetros deben ser de tipo Partida
int compararPartidas(const void *a, const void *b) {
    const Partida partida1 = *(Partida *)a;
    const Partida partida2 = *(Partida *)b;

    if (pNeto(partida1) > pNeto(partida2)) return -1;
    if (pNeto(partida1) < pNeto(partida2)) return 1;

    if (partida1.puntajeJugador > partida2.puntajeJugador) return -1;
    if (partida1.puntajeJugador < partida2.puntajeJugador) return 1;

    if (partida1.size > partida2.size) return -1;
    if (partida1.size < partida2.size) return 1;

    if (partida1.fecha > partida2.fecha) return -1;
    if (partida1.fecha < partida2.fecha) return 1;

    return 0;
}
