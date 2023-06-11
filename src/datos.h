#include <stdbool.h>

#ifndef _DATOS_DEFINIDO

#define _DATOS_DEFINIDO
/**
 * Máximo de 32 caracteres, 1 extra para el nulo
 */
#define NOMBRE_MAX 33
#define TABLERO_MAX 19

typedef struct GoConfig {
    char nombre[NOMBRE_MAX];
} GoConfig;

extern GoConfig config;

typedef unsigned short t_char;

typedef struct GoPartida {
    bool terminada;
    bool victoria;
    int puntajeJugador;
    int puntajeMaquina;
    int size;
    t_char tablero[TABLERO_MAX][TABLERO_MAX];
} GoPartida;

void setupDatos();
void guardarConfig();
void guardarPartida(GoPartida partida);
// void cargarPartida();
bool validarNombre(char *nombre);

#endif
