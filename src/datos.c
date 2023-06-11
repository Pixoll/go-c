#include "datos.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define LEER "rb"
#define ESCRIBIR "wb"
#define LEER_ESCRIBIR "rb+"
#define ANEXAR "ab+"

const char *rutaConfig = "./config.bin";
const char *rutaPartidas = "./partidas.bin";
FILE *archivoConfig;

const size_t configSize = sizeof(GoConfig);
const size_t partidaSize = sizeof(GoPartida);
GoConfig config;

void setupDatos() {
    archivoConfig = fopen(rutaConfig, LEER);
    if (!archivoConfig) {
        archivoConfig = fopen(rutaConfig, ESCRIBIR);
        fclose(archivoConfig);
    } else {
        fread(&config, configSize, 1, archivoConfig);
        fclose(archivoConfig);
    }

    FILE *archivoPartidas = fopen(rutaPartidas, LEER_ESCRIBIR);
    if (!archivoPartidas) {
        archivoPartidas = fopen(rutaPartidas, ESCRIBIR);
        fclose(archivoPartidas);
    }
}

void guardarConfig() {
    archivoConfig = fopen(rutaConfig, LEER_ESCRIBIR);
    const int saved = fwrite(&config, configSize, 1, archivoConfig);
    if (!saved) {
        perror("error while saving");
        exit(1);
    }
    fclose(archivoConfig);
}

// TODO: Por ahora solo puede almacenar una, añadir funcionalidad para añadir multiples y editar existentes
void guardarPartida(GoPartida partida) {
    FILE *archivoPartidas = fopen(rutaPartidas, LEER_ESCRIBIR);
    const int saved = fwrite(&partida, partidaSize, 1, archivoPartidas);
    if (!saved) {
        perror("error while saving");
        exit(1);
    }
    fclose(archivoPartidas);
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
