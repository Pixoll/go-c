#include <stdbool.h>
#include <wchar.h>

#ifndef _DATOS_DEFINIDO

#define _DATOS_DEFINIDO
/**
 * Máximo de 32 caracteres, 1 extra para el nulo
 */
#define NOMBRE_MAX 33

#define TABLERO_S 9
#define TABLERO_M 13
#define TABLERO_L 19
#define TABLERO_MAX 21
#define TABLEROS 3

typedef struct GoConfig {
    char nombre[NOMBRE_MAX];
} GoConfig;

extern GoConfig config;

typedef struct GoPartida {
    bool terminada;
    bool victoria;
    bool turnoNegras;
    int puntajeJugador;
    int puntajeMaquina;
    int size;
    wchar_t tablero[TABLERO_MAX][TABLERO_MAX];
} GoPartida;

void setupDatos();
void guardarConfig();
bool validarNombre(char *nombre);
const GoPartida *ultimaPartida();
void guardarPartida(GoPartida partida);
// void cargarPartida();
void borrarTodasPartidas();

#endif
