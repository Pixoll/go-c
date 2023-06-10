#include <stdbool.h>

#ifndef _DATOS_DEFINIDO

#define _DATOS_DEFINIDO
/**
 * Máximo de 32 caracteres, 1 extra para el nulo
 */
#define NOMBRE_MAX 33

typedef struct GoConfig {
    char nombre[NOMBRE_MAX];
} GoConfig;

extern GoConfig config;

typedef unsigned short t_char;

typedef struct GoPartida {
    int size;
    t_char **tablero;
} GoPartida;

void setupDatos();
void guardarConfig();
bool validarNombre(char *nombre);

#endif
