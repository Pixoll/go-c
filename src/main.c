#include <stdio.h>
#include <string.h>

// Consola en español
#include <locale.h>
#include <wchar.h>

// Archivos locales
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

int obtenerMenu();
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
    printf("\t\tBienvenido %s!\n\n", config.nombre);

    const int menu = obtenerMenu();
    if (menu == MENU_JUGAR) {
        const int size = selectTablero();
        wprintf(L"Tamaño seleccionado: %dx%d\n\n", size, size);

        crearTablero(size);
        jugarTablero();
        return 0;
    }

    return 0;
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

int selectTablero() {
    wprintf(L"Selecciona el tamaño del tablero.\n");
    for (int i = 0; i < TABLEROS; i++) {
        const int size = tableros[i];
        printf("%d. %dx%d\n", i + 1, size, size);
    }

    int size;
    wprintf(L"Ingresa el tamaño aquí: ");

    while (!getInt(&size) || size < 1 || size > TABLEROS) {
        wprintf(L"Tamaño inválido. Intenta de nuevo: ");
    }

    return tableros[size - 1];
}
