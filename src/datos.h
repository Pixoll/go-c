#include <stdbool.h>

#ifndef _DATOS_DEFINIDO

#define _DATOS_DEFINIDO
/**
 * Máximo de 32 caracteres, 1 extra para el nulo
 */
#define NOMBRE_MAX 33

#define TABLERO_S 9
#define TABLERO_M 13
#define TABLERO_L 19
#define TABLERO_MAX TABLERO_L
#define TABLEROS 3

typedef struct GoConfig {
    char nombre[NOMBRE_MAX];
} GoConfig;

extern GoConfig config;

typedef struct GoPartida {
    bool terminada;
    bool turnoNegras;
    int puntajeJugador;
    /* Vacío si es la máquina */
    char oponente[NOMBRE_MAX];
    int puntajeOponente;
    unsigned long fecha;
    int size;
    char tablero[TABLERO_MAX][TABLERO_MAX];
} GoPartida;

typedef struct TodasGoPartidas {
    int numero;
    GoPartida *partidas;
} TodasGoPartidas;

void setupDatos();
void guardarConfig();
void borrarConfig();
bool validarNombre(const char *nombre);
GoPartida *obtenerUltimaPartida();
void borrarUltimaPartida();
void guardarPartida(const  GoPartida partida, bool reemplazarUltima);
TodasGoPartidas obtenerTodasPartidas(bool soloTerminadas);
void borrarTodasPartidas();

#endif
