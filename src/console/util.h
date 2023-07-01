#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>

#ifndef _CONSOLE_UTIL_DEFINIDO

#define _CONSOLE_UTIL_DEFINIDO

// signo + 5 dígitos + nulo
#define TITULO_LEN 80

void wprintCentro(const wchar_t *texto, int sizeLinea);
void wprintConLineLimit(const wchar_t *texto, int limit);
void printCentro(const char *texto, int sizeLinea);
void printTitulo();
char *strget(char *buffer, int size, char termino);
bool getInt(int *n);
void limpiarConsola();
bool confirmar(const wchar_t *action, bool predeterminado);
void esperar(unsigned int segundos);

#endif
