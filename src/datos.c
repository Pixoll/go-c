#include "datos.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#define LEER "rb"
#define CREAR "wb"
#define LEER_ESCRIBIR "rb+"

const char *rutaConfig = "./config.bin";
const char *rutaPartidas = "./partidas.bin";

const int configSize = sizeof(GoConfig);
const int partidaSize = sizeof(GoPartida);
GoConfig config;

void setupDatos() {
    FILE *archivoConfig = fopen(rutaConfig, LEER);
    if (!archivoConfig)
        archivoConfig = fopen(rutaConfig, CREAR);
    else
        fread(&config, configSize, 1, archivoConfig);
    fclose(archivoConfig);

    FILE *archivoPartidas = fopen(rutaPartidas, LEER);
    if (!archivoPartidas)
        archivoPartidas = fopen(rutaPartidas, CREAR);
    fclose(archivoPartidas);
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

void borrarConfig() {
    strempty(config.nombre);
}

bool validarNombre(const char *nombre) {
    const int size = strlen(nombre);
    if (size == 0) return false;
    for (int i = 0; i < size; i++) {
        const char c = nombre[i];
        // Letra, número o guion bajo
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_') continue;
        return false;
    }
    return true;
}

GoPartida *obtenerUltimaPartida() {
    GoPartida *ultimaPartida = malloc(partidaSize);
    FILE *archivoPartidas = fopen(rutaPartidas, LEER);
    if (fseek(archivoPartidas, -partidaSize, SEEK_END) != 0)
        return NULL;

    fread(ultimaPartida, partidaSize, 1, archivoPartidas);
    fclose(archivoPartidas);
    return ultimaPartida;
}

void guardarPartida(const GoPartida partida) {
    FILE *archivoPartidas = fopen(rutaPartidas, LEER_ESCRIBIR);
    GoPartida *partidaGuardada = obtenerUltimaPartida();
    if (partidaGuardada && partidaGuardada->terminada)
        fseek(archivoPartidas, 0, SEEK_END);
    free(partidaGuardada);

    const int guardado = fwrite(&partida, partidaSize, 1, archivoPartidas);
    if (!guardado) {
        perror("Error al intentar guardar");
        exit(1);
    }
    fclose(archivoPartidas);
}

TodasGoPartidas obtenerTodasPartidas(bool soloTerminadas) {
    TodasGoPartidas todasPartidas = {0};
    todasPartidas.partidas = malloc(partidaSize);
    unsigned long int size = 0;
    FILE *archivoPartidas = fopen(rutaPartidas, LEER);
    if (!archivoPartidas) return todasPartidas;

    fseek(archivoPartidas, 0, SEEK_END);
    const long max = ftell(archivoPartidas);
    fseek(archivoPartidas, 0, SEEK_SET);
    while (size < max) {
        GoPartida partida;
        fread(&partida, partidaSize, 1, archivoPartidas);
        size += partidaSize;
        if (soloTerminadas && !partida.terminada) continue;

        todasPartidas.partidas = realloc(todasPartidas.partidas, (todasPartidas.numero + 1) * partidaSize);
        todasPartidas.partidas[todasPartidas.numero++] = partida;
    }

    fclose(archivoPartidas);
    return todasPartidas;
}

void borrarTodasPartidas() {
    FILE *archivoPartidas = fopen(rutaPartidas, LEER);
    if (!archivoPartidas) return;
    fclose(archivoPartidas);
    archivoPartidas = fopen(rutaPartidas, CREAR);
    fclose(archivoPartidas);
}
