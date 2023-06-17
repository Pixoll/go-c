#include "menu.h"

#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "datos.h"
#include "tablero.h"
#include "util.h"

const wchar_t *menus[MENUS] = {
    L"Jugar",
    L"Reglas",
    L"Configuración",
    L"Estadísticas",
};
const wchar_t *configs[CONFIGS] = {
    L"Cambiar nombre",
    L"Borrar partidas guardadas",
    L"Borrar todos los datos guardados (reinicia el juego)",
};
const wchar_t *stats[STATS] = {
    L"Fecha",
    L"Tablero",
    L"Oponente",
    L"P. Jugador",
    L"P. Oponente",
    L"P. Neto",
};
const int statsPad = 12;
const int tableros[TABLEROS] = {TABLERO_S, TABLERO_M, TABLERO_L};

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

int obtenerTableroSize();

int ejecutarMenuJugar() {
    const int size = obtenerTableroSize();
    if (size == VOLVER) return VOLVER;
    wprintf(L"Tamaño seleccionado: %dx%d\n\n", size, size);

    crearTablero(size);
    jugarTablero();
    return SALIR;
}

int obtenerConfig();

int ejecutarMenuConfig() {
    const int menuConfig = obtenerConfig();
    if (menuConfig == VOLVER) return VOLVER;
    wprintf(L"Configuración seleccionada: %ls\n\n", configs[menuConfig]);

    if (menuConfig == CONFIG_NOMBRE) {
        strempty(config.nombre);
        obtenerNombre();
        return VOLVER;
    }
    if (menuConfig == CONFIG_BORRAR_PARTIDAS) {
        borrarTodasPartidas();
        return VOLVER;
    }

    return VOLVER;
}

void printStats(const TodasGoPartidas todasPartidas, int page);

int ejecutarMenuStats() {
    const TodasGoPartidas todasPartidas = obtenerTodasPartidas(true);
    const GoPartida *partidas = todasPartidas.partidas;
    const int numero = todasPartidas.numero;
    const int pages = numero == STATS_POR_PAGE ? 1 : numero / STATS_POR_PAGE + 1;

    if (numero > 0)
        printStats(todasPartidas, 0);
    else
        wprintf(L"Aún no tienes alguna partida guardada.\n");

    int i = 1, next = -1, page = 0;
    if (pages > 1) {
        wprintf(L"\n%d. Página siguiente", i++);
        next = i;
    }
    wprintf(L"\n%d. Volver al menú principal\n", i);

    int opcion;
    wprintf(L"\nSelecciona la opción: ");

    while (!getInt(&opcion) || opcion < 1 || opcion > i) {
        wprintf(L"Opción inválida. Intenta de nuevo: ");
    }

    while (opcion != i) {
        limpiarConsola();
        printTitulo();
        printStats(todasPartidas, i == next ? ++page : --page);
        next = -1;

        int i = 1;
        if (page != 0)
            wprintf(L"\n%d. Página anterior", i++);
        if (page != pages - 1) {
            wprintf(L"\n%d. Página siguiente", i++);
            next = i;
        }

        wprintf(L"\n%d. Volver al menú principal\n", i);
        wprintf(L"\nSelecciona la opción: ");

        while (!getInt(&opcion) || opcion < 1 || opcion > i) {
            wprintf(L"Opción inválida. Intenta de nuevo: ");
        }
    }

    return VOLVER;
}

void printStats(const TodasGoPartidas todasPartidas, int page) {
    const int numero = todasPartidas.numero;
    const GoPartida *partidas = todasPartidas.partidas;
    printf("   ");
    for (int i = 0; i < STATS; i++)
        wprintf(wcspadright((wchar_t *)stats[i], statsPad, ' '));
    printf("\n");

    const int inicio = page * STATS_POR_PAGE;
    const int fin = inicio + STATS_POR_PAGE;

    for (int i = inicio; i < numero && i < fin; i++) {
        GoPartida partida = partidas[i];
        char tablero[6];
        sprintf(tablero, "%dx%d", partida.size, partida.size);
        wchar_t *oponente = partida.oponente[0] == '\0' ? L"Máquina" : strtowcs(partida.oponente);

        wprintf(
            L"%d. %s%s%ls%s%s%s\n",
            i + 1,
            strpadright("test", statsPad, ' '),
            strpadright(tablero, statsPad, ' '),
            wcspadright(oponente, statsPad, ' '),
            strpadright(intATexto(partida.puntajeJugador), statsPad, ' '),
            strpadright(intATexto(partida.puntajeOponente), statsPad, ' '),
            strpadright(intATexto(partida.puntajeJugador - partida.puntajeOponente), statsPad, ' '));
    }
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

    if (size == TABLEROS + 1) return VOLVER;
    return tableros[size - 1];
}

int obtenerConfig() {
    for (int i = 0; i < CONFIGS; i++)
        wprintf(L"%d. %ls\n", i + 1, configs[i]);
    wprintf(L"%d. Volver al menú principal\n\n", CONFIGS + 1);

    int menuConfig;
    wprintf(L"Selecciona la configuración: ");

    while (!getInt(&menuConfig) || menuConfig < 1 || menuConfig > CONFIGS + 1) {
        wprintf(L"Configuración inválida. Intenta de nuevo: ");
    }

    if (menuConfig == CONFIGS + 1) return VOLVER;
    return menuConfig - 1;
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
