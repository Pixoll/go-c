#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define LEER "rb"
#define ESCRIBIR "wb"
#define LEER_ESCRIBIR "rb+"

#define NOMBRE_MAX 32

const char *rutaConfig = "./config.bin";
FILE *archivoConfig;

typedef struct Config {
    char nombre[NOMBRE_MAX];
} Config;

Config config;

void setupDatos() {
    archivoConfig = fopen(rutaConfig, LEER);
    if (archivoConfig == NULL) {
        archivoConfig = fopen(rutaConfig, ESCRIBIR);
        fclose(archivoConfig);
        return;
    }

    fread(&config, sizeof(config), 1, archivoConfig);
    fclose(archivoConfig);
}

Config obtenerConfig() {
    return config;
}

void guardarDatos() {
    archivoConfig = fopen(rutaConfig, LEER_ESCRIBIR);
    const int saved = fwrite(&config, sizeof(config), 1, archivoConfig);
    if (!saved) {
        perror("error while saving");
        exit(1);
    }
    fclose(archivoConfig);
}

void guardarNombre(char *nombre) {
    strcpy(config.nombre, nombre);
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
