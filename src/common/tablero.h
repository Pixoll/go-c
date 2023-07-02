#include "datos.h"

#ifndef _COMMON_TABLERO_DEFINIDO

#define _COMMON_TABLERO_DEFINIDO

#define CELDAS 8
enum CELDA {
    CELDA_NEGRA,
    CELDA_BLANCA,
    CELDA_EMPTY_HOR,
    CELDA_EMPTY_VERT,
    CELDA_NEGRA_CAPT,
    CELDA_BLANCA_CAPT,
    CELDA_EMPTY,
    CELDA_ANALIZADA,
};
typedef enum CELDA celda_t;

extern Partida partida;

void crearPartida(int size, char *oponente);
void cargarPartida(Partida *guardada);
bool esMaquina();
bool estaOcupada(int x, int y);
void marcarOcupada(int x, int y);
void jugarMaquina(int *px, int *py);
void puntajePorCantidad();
void puntajePorArea();
bool capturas(celda_t celdaJugador, celda_t celdaOponente);
void eliminarCapturadas();
bool ko(int x, int y, celda_t celdaJugador, celda_t celdaOponente);
bool suicidio(int x, int y, celda_t celdaJugador, celda_t celdaOponente);

#endif
