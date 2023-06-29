#include "datos.h"

#ifndef _TABLERO_DEFINIDO

#define _TABLERO_DEFINIDO

int crearPartida(int size, char *oponente);
void cargarPartida(Partida *guardada);
void printTablero();
void jugarPartida(bool cargada);

#endif
