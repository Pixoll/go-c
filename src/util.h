#ifndef _UTIL_DEFINIDO

#define _UTIL_DEFINIDO

#define TITULO_LEN 50

void wprintCentro(wchar_t *texto, int sizeLinea);
void printCentro(char *texto, int sizeLinea);
void printTitulo();
char *strrepeat(char fill, int size);
char *strempty(char *buffer);
char *strget(char *buffer, int size);
char *strpadleft(char *buffer, int n, char fill);
bool getInt(int *n);
void limpiarConsola();
unsigned long now();

#endif
