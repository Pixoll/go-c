#include <stdio.h>
#include <string.h>

// Consola en español
#include <locale.h>
#include <wchar.h>

// Archivos locales
#include "datos.h"
#include "tablero.h"
#include "util.h"


const int tableroSizes[TABLEROS] = {TABLERO_S, TABLERO_M, TABLERO_L};

int selectTablero();
void obtenerNombre();

void setup() {
    setupDatos();
    setlocale(LC_ALL, "");
}

int main() {
    setup();
    printTitulo();

    obtenerNombre();
    printf("Hola %s!\n\n", config.nombre);

    const int size = selectTablero();
    wprintf(L"Tamaño seleccionado: %dx%d\n\n", size, size);

    crearTablero(size);
    jugarTablero();

    return 0;
}

void obtenerNombre() {
    if (strlen(config.nombre)) return;

    wprintf(L"¿Cuál es tu nombre de usuario? Máximo %d caracteres (alfanuméricos y guion bajo).\n", NOMBRE_MAX - 1);
    char nombre[NOMBRE_MAX];
    strget(nombre, NOMBRE_MAX);

    while (!validarNombre(nombre)) {
        wprintf(L"Nombre inválido. Intenta de nuevo: ");
        strget(nombre, NOMBRE_MAX);
    }

    strcpy(config.nombre, nombre);
    guardarConfig();
}

int selectTablero() {
    wprintf(L"Seleccione el tamaño del tablero.\n");
    for (int i = 0; i < TABLEROS; i++) {
        const int size = tableroSizes[i];
        printf("%d. %dx%d\n", i + 1, size, size);
    }

    int size;
    wprintf(L"Ingrese el tamaño aquí: ");

    while (!getInt(&size) || size < 1 || size > TABLEROS) {
        wprintf(L"Tamaño inválido. Intente de nuevo: ");
    }

    return tableroSizes[size - 1];
}
