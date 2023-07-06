#include <stdio.h>
#include <wchar.h>

#ifndef _COMMON_UTIL_DEFINIDO

#define _COMMON_UTIL_DEFINIDO

// signo + 5 dígitos + nulo
#define INT_STR_MAX 7

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define par(a) !((a) & 1)
#define pNeto(partida) (partida.puntajeJugador - partida.puntajeOponente)

char *strrepeat(char fill, int size);
char *strempty(char *buffer);
char *substr(char *buffer, unsigned int inicio, unsigned int fin);
wchar_t *strtowcs(const char *buffer);
wchar_t *wcslower(const wchar_t* buffer);
int intDigits(int n);
long long now();
char *obtenerFecha(long long ms);
void swap(int *px, int *py);
int compararPartidas(const void *a, const void *b);

#endif
