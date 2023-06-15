#ifndef _UTIL_DEFINIDO

#define _UTIL_DEFINIDO

#define TITULO_LEN 50

void wprintCentro(wchar_t *texto, int sizeLinea);
void printCentro(char *texto, int sizeLinea);
void printTitulo();
char *strrepeat(char fill, int size);
char *strempty(char *buffer);
char *strget(char *buffer, int size);
bool getInt(int *n);
void limpiarConsola();

#endif
