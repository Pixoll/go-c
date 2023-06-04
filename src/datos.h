#include <stdbool.h>

#define NOMBRE_MAX 32

typedef struct Datos {
    char nombre[NOMBRE_MAX];
} Datos;

void setupDatos();
Datos obtenerDatos();
void guardarNombre(char *nombre);
bool validarNombre(char *nombre);
