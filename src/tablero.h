#include "datos.h"

#ifndef _TABLERO_DEFINIDO

#define _TABLERO_DEFINIDO

extern t_char **tablero;
extern int tableroSize;

int crearTablero(int size);
void printTablero();
void jugarTablero();

#endif
