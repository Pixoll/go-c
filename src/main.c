#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#include "datos.h"
#include "tablero.h"
#include "util.h"

#define MENU_JUGAR 0
#define MENU_REGLAS 1
#define MENU_CONFIG 2
#define MENU_STATS 3
#define MENUS 4

const wchar_t *menus[MENUS] = {L"Jugar", L"Reglas", L"Configuración", L"Estadísticas"};
const int tableros[TABLEROS] = {TABLERO_S, TABLERO_M, TABLERO_L};

void printBienvenida();
void obtenerNombre();
int obtenerMenu();
int obtenerTableroSize();

void setup() {
    setupDatos();
    setlocale(LC_ALL, "");
}

int main() {
    setup();
    obtenerNombre();

    while (true) {
        printBienvenida();
        const int menu = obtenerMenu();
        if (menu == MENU_JUGAR) {
            const int size = obtenerTableroSize();
            if (size == -1) continue;
            wprintf(L"Tamaño seleccionado: %dx%d\n\n", size, size);

            crearTablero(size);
            jugarTablero();
            break;
        }
    }

    return 0;
}

void printBienvenida() {
    limpiarConsola();
    printTitulo();

    const int sizeBienvenida = 13 + NOMBRE_MAX;
    wchar_t bienvenido[sizeBienvenida];
    swprintf(bienvenido, sizeBienvenida, L"¡Bienvenid@ %s!", config.nombre);

    wprintCentro(bienvenido, 50);
    printf("\n");
}

int obtenerMenu() {
    for (int i = 0; i < MENUS; i++)
        wprintf(L"%d. %ls\n", i + 1, menus[i]);

    int menu;
    wprintf(L"\nSelecciona el menú: ");

    while (!getInt(&menu) || menu < 1 || menu > MENUS) {
        wprintf(L"Menú inválido. Intenta de nuevo: ");
    }

    limpiarConsola();
    printTitulo();

    return menu - 1;
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

int obtenerTableroSize() {
    for (int i = 0; i < TABLEROS; i++) {
        const int size = tableros[i];
        printf("%d. %dx%d\n", i + 1, size, size);
    }
    wprintf(L"%d. Volver al menú principal\n\n", TABLEROS + 1);

    int size;
    wprintf(L"Selecciona el tamaño del tablero: ");

    while (!getInt(&size) || size < 1 || size > TABLEROS + 1) {
        wprintf(L"Tamaño inválido. Intenta de nuevo: ");
    }

    if (size == TABLEROS + 1) return -1;
    return tableros[size - 1];
}
