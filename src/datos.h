#include <stdbool.h>

#ifndef _DATOS_DEFINIDO

#define _DATOS_DEFINIDO
#define NOMBRE_MAX 32

typedef struct Datos {
    char nombre[NOMBRE_MAX];
} Datos;

void setupDatos();
Datos obtenerDatos();
void guardarNombre(char *nombre);
bool validarNombre(char *nombre);

#endif
