#include "menu.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datos.h"
#include "tablero.h"
#include "util.h"

#define pNeto(partida) (partida.puntajeJugador - partida.puntajeOponente)

const char *rutaReglas = "./reglas.txt";

const wchar_t *menus[MENUS] = {
    L"Jugar",
    L"Reglas",
    L"Configuración",
    L"Estadísticas",
};
const wchar_t *configs[CONFIGS] = {
    L"Cambiar tu nombre de usuario",
    L"Borrar partidas guardadas",
    L"Borrar todos los datos guardados",
};
const wchar_t *stats[STATS] = {
    L"Fecha",
    L"Tablero",
    L"Oponente",
    L"P. Jugador",
    L"P. Oponente",
    L"P. Neto",
};

#define STATS_PAD 13

const int tableros[TABLEROS] = {TABLERO_S, TABLERO_M, TABLERO_L};

void obtenerNombre() {
    if (strlen(config.nombre)) return;

    wprintf(L"¿Cuál es tu nombre de usuario? Máximo %d caracteres (alfanuméricos y guion bajo).\n", NOMBRE_MAX - 1);
    char nombre[NOMBRE_MAX];
    strget(nombre, NOMBRE_MAX);

    while (!validarNombre(nombre)) {
        wprintf(L"Nombre inválido. Intenta de nuevo: ");
        strget(nombre, NOMBRE_MAX);
    }

    strncpy(config.nombre, nombre, NOMBRE_MAX);
    guardarConfig();
}

void printBienvenida(bool repetirFlag) {
    if (!repetirFlag) obtenerNombre();
    limpiarConsola();
    printTitulo();

    if (repetirFlag) return;

    const wchar_t *formato = L"¡Bienvenid@ %s!";
    const int sizeBienvenida = wcslen(formato) - 3 + NOMBRE_MAX;
    wchar_t bienvenido[sizeBienvenida];
    swprintf(bienvenido, sizeBienvenida, formato, config.nombre);

    wprintCentro(bienvenido, TITULO_LEN);
    printf("\n");
}

int obtenerMenu() {
    for (int i = 0; i < MENUS; i++)
        wprintf(L"%d. %ls\n", i + 1, menus[i]);
    wprintf(L"%d. Salir del juego\n", MENUS + 1);

    int menu;
    wprintf(L"\nSelecciona el menú: ");

    while (!getInt(&menu) || menu < 1 || menu > MENUS + 1) {
        wprintf(L"Menú inválido. Intenta de nuevo: ");
    }

    if (menu == MENUS + 1) return SALIR;

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
    return VOLVER;
}

int ejecutarMenuReglas() {
    FILE *archivoReglas = fopen(rutaReglas, "r");
    fseek(archivoReglas, 0, SEEK_END);
    const long max = ftello(archivoReglas) + 1;
    fseek(archivoReglas, 0, SEEK_SET);

    char reglas[max];
    fread(reglas, 1, max, archivoReglas);
    fclose(archivoReglas);

    printCentro("-== REGLAS DE GO ==-\n", TITULO_LEN);
    wprintConLineLimit(strtowcs(reglas), TITULO_LEN);

    wprintf(L"\n\nPresiona ENTER para volver al menú principal.");
    char c = 0;
    while (c != '\n') scanf("%c", &c);

    return VOLVER;
}

int obtenerConfig();

int ejecutarMenuConfig() {
    const int menuConfig = obtenerConfig();
    if (menuConfig == VOLVER) return VOLVER;

    if (menuConfig == CONFIG_NOMBRE) {
        strempty(config.nombre);
        obtenerNombre();
        return REPETIR;
    }

    if (menuConfig == CONFIG_BORRAR_PARTIDAS) {
        const bool confirmado = confirmar(wcslower(configs[menuConfig]));
        if (confirmado) borrarTodasPartidas();
        return REPETIR;
    }

    if (menuConfig == CONFIG_BORRAR_TODO) {
        const bool confirmado = confirmar(wcslower(configs[menuConfig]));
        if (!confirmado) return REPETIR;
        borrarTodasPartidas();
        borrarConfig();
        return VOLVER;
    }

    return VOLVER;
}

int compararPartidas(const void *a, const void *b);
void printStats(const TodasGoPartidas todasPartidas, int page);

int ejecutarMenuStats() {
    const TodasGoPartidas todasPartidas = obtenerTodasPartidas(true);
    const int numero = todasPartidas.numero;
    const int pages = numero == STATS_POR_PAGE ? 1 : numero / STATS_POR_PAGE + 1;
    qsort(todasPartidas.partidas, numero, sizeof(GoPartida), compararPartidas);

    if (numero > 0)
        printStats(todasPartidas, 0);
    else
        wprintf(L"Aún no tienes alguna partida guardada.\n");

    int i = 1, next = -1, page = 0;
    if (pages > 1) {
        next = i;
        wprintf(L"\n%d. Página siguiente", i++);
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
        printStats(todasPartidas, opcion == next ? ++page : --page);
        next = -1;

        i = 1;
        if (page != 0)
            wprintf(L"\n%d. Página anterior", i++);
        if (page != pages - 1) {
            next = i;
            wprintf(L"\n%d. Página siguiente", i++);
        }

        wprintf(L"\n%d. Volver al menú principal\n", i);
        wprintf(L"\nSelecciona la opción: ");

        while (!getInt(&opcion) || opcion < 1 || opcion > i) {
            wprintf(L"Opción inválida. Intenta de nuevo: ");
        }
    }

    return VOLVER;
}

int compararPartidas(const void *a, const void *b) {
    const GoPartida partida1 = *(GoPartida *)a;
    const GoPartida partida2 = *(GoPartida *)b;

    if (pNeto(partida1) > pNeto(partida2)) return -1;
    if (pNeto(partida1) < pNeto(partida2)) return 1;

    if (partida1.puntajeJugador > partida2.puntajeJugador) return -1;
    if (partida1.puntajeJugador < partida2.puntajeJugador) return 1;

    if (partida1.size > partida2.size) return -1;
    if (partida1.size < partida2.size) return 1;

    if (partida1.fecha > partida2.fecha) return -1;
    if (partida1.fecha < partida2.fecha) return 1;

    return 0;
}

void printStats(const TodasGoPartidas todasPartidas, int page) {
    const int numero = todasPartidas.numero;
    const GoPartida *partidas = todasPartidas.partidas;

    const int inicio = page * STATS_POR_PAGE;
    const int fin = inicio + STATS_POR_PAGE;
    const int idPad = intDigits(max(fin, numero));

    printf("%s", strrepeat(' ', idPad + 2));
    for (int i = 0; i < STATS; i++)
        wprintf(wcspadright(stats[i], STATS_PAD, ' '));
    printf("\n");

    for (int i = inicio; i < numero && i < fin; i++) {
        GoPartida partida = partidas[i];
        char tablero[6];
        snprintf(tablero, 6, "%dx%d", partida.size, partida.size);
        wchar_t *oponente = partida.oponente[0] == '\0' ? L"Máquina" : strtowcs(partida.oponente);

        wprintf(
            L"%s. %s%s%ls%s%s%s\n",
            strpadleft(intATexto(i + 1), idPad, ' '),
            strpadright(obtenerFecha(partida.fecha), STATS_PAD, ' '),
            strpadright(tablero, STATS_PAD, ' '),
            wcspadright(oponente, STATS_PAD, ' '),
            strpadright(intATexto(partida.puntajeJugador), STATS_PAD, ' '),
            strpadright(intATexto(partida.puntajeOponente), STATS_PAD, ' '),
            strpadright(intATexto(pNeto(partida)), STATS_PAD, ' '));
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
