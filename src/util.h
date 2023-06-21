#include <stdbool.h>
// Para Linux
#include <stddef.h>
#include <stdio.h>

#ifndef _UTIL_DEFINIDO

#define _UTIL_DEFINIDO

#define TITULO_LEN 80

void wprintCentro(const wchar_t *texto, int sizeLinea);
void wprintConLineLimit(const wchar_t *texto, int limit);
void printCentro(const char *texto, int sizeLinea);
void printTitulo();
char *strrepeat(char fill, int size);
char *strempty(char *buffer);
char *strget(char *buffer, int size);
char *strpadleft(const char *buffer, int max, char fill);
char *strpadright(const char *buffer, int n, char fill);
wchar_t *wcsrepeat(wchar_t fill, int size);
wchar_t *wcspadright(const wchar_t *buffer, int max, wchar_t fill);
wchar_t *strtowcs(const char *buffer);
bool getInt(int *n);
char *intATexto(int n);
int intDigits(int n);
void limpiarConsola();
long long now();
char *obtenerFecha(long long ms);
bool confirmar(const wchar_t *action);

#endif
