#include "datos.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define LEER "rb"
#define ESCRIBIR "wb"
#define LEER_ESCRIBIR "rb+"

const char *rutaConfig = "./config.bin";
const char *rutaPartidas = "./partidas.bin";

const int configSize = sizeof(GoConfig);
const int partidaSize = sizeof(GoPartida);
GoConfig config;

void setupDatos() {
    FILE *archivoConfig = fopen(rutaConfig, LEER);
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
    FILE *archivoConfig = fopen(rutaConfig, LEER_ESCRIBIR);
    const int guardado = fwrite(&config, configSize, 1, archivoConfig);
    if (!guardado) {
        perror("Error al intentar guardar");
        exit(1);
    }
    fclose(archivoConfig);
}

const GoPartida *ultimaPartida() {
    FILE *archivoPartidas = fopen(rutaPartidas, LEER);
    if (fseek(archivoPartidas, -partidaSize, SEEK_END) == 0) {
        GoPartida ultimaPartida;
        fread(&ultimaPartida, partidaSize, 1, archivoPartidas);
        fclose(archivoPartidas);
        const GoPartida *puntero = &ultimaPartida;
        return puntero;
    }
    return NULL;
}

void guardarPartida(GoPartida partida) {
    const GoPartida *partidaGuardada = ultimaPartida();
    FILE *archivoPartidas = fopen(rutaPartidas, LEER_ESCRIBIR);
    if (partidaGuardada != NULL && partidaGuardada->terminada)
        fseek(archivoPartidas, 0, SEEK_END);

    const int guardado = fwrite(&partida, partidaSize, 1, archivoPartidas);
    if (!guardado) {
        perror("Error al intentar guardar");
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
