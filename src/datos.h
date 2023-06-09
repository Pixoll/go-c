#include <stdbool.h>

#ifndef _DATOS_DEFINIDO

#define _DATOS_DEFINIDO
#define NOMBRE_MAX 32

typedef struct Config {
    char nombre[NOMBRE_MAX];
} Config;

void setupDatos();
Config obtenerConfig();
void guardarNombre(char *nombre);
bool validarNombre(char *nombre);

#endif
