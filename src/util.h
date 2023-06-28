#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>

#ifndef _UTIL_DEFINIDO

#define _UTIL_DEFINIDO

// signo + 5 dígitos + nulo
#define INT_STR_MAX 7
#define TITULO_LEN 80

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define par(a) !((a) & 1)

void wprintCentro(const wchar_t *texto, int sizeLinea);
void wprintConLineLimit(const wchar_t *texto, int limit);
void printCentro(const char *texto, int sizeLinea);
void printTitulo();
char *strrepeat(char fill, int size);
char *strempty(char *buffer);
char *strget(char *buffer, int size, char termino);
wchar_t *strtowcs(const char *buffer);
wchar_t *wcslower(const wchar_t* buffer);
bool getInt(int *n);
int intDigits(int n);
void limpiarConsola();
long long now();
char *obtenerFecha(long long ms);
bool confirmar(const wchar_t *action, bool predeterminado);
void esperar(unsigned int segundos);

#endif
