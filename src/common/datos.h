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

typedef struct Config {
    char nombre[NOMBRE_MAX];
} Config;

extern Config config;

typedef struct Partida {
    bool terminada;
    int turno;
    int puntajeJugador;
    /* Vacío si es la máquina */
    char oponente[NOMBRE_MAX];
    int puntajeOponente;
    long long fecha;
    int size;
    char tablero[TABLERO_MAX][TABLERO_MAX];
    char anterior[TABLERO_MAX][TABLERO_MAX];
} Partida;

typedef struct TodasPartidas {
    int numero;
    Partida *partidas;
} TodasPartidas;

void setupDatos();
void guardarConfig();
void borrarConfig();
bool validarNombre(const char *nombre);
Partida *obtenerUltimaPartida();
void borrarUltimaPartida();
void guardarPartida(const Partida partida, bool reemplazarUltima);
TodasPartidas obtenerTodasPartidas(bool soloTerminadas);
void borrarTodasPartidas();

#endif
