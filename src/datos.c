#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define LEER "rb"
#define ESCRIBIR "wb"
#define LEER_ESCRIBIR "rb+"

#define NOMBRE_MAX 32

const char *rutaArchivo = "./data.bin";
FILE *archivo;

typedef struct Datos {
    char nombre[NOMBRE_MAX];
} Datos;

Datos datos;

void setupDatos() {
    archivo = fopen(rutaArchivo, LEER);
    if (archivo == NULL) {
        archivo = fopen(rutaArchivo, ESCRIBIR);
        fclose(archivo);
        return;
    }

    fread(&datos, sizeof(datos), 1, archivo);
    fclose(archivo);
}

Datos obtenerDatos() {
    return datos;
}

void guardarDatos() {
    archivo = fopen(rutaArchivo, LEER_ESCRIBIR);
    const int saved = fwrite(&datos, sizeof(datos), 1, archivo);
    if (!saved) {
        perror("error while saving");
        exit(1);
    }
    fclose(archivo);
}

void guardarNombre(char *nombre) {
    strcpy(datos.nombre, nombre);
    guardarDatos();
}

bool validarNombre(char *nombre) {
    const int size = strlen(nombre);
    for (int i = 0; i < size; i++) {
        const char c = nombre[i];
        // Letra, número o guion bajo
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_') continue;
        return false;
    }
    return true;
}
