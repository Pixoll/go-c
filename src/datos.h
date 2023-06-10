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

void setupDatos();
void guardarConfig();
bool validarNombre(char *nombre);

#endif