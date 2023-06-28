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
const wchar_t *oponentes[OPONENTES] = {
    L"Máquina",
    L"Otro jugador",
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
    const int sizeBienvenida = wcslen(formato) - 2 + NOMBRE_MAX;
    wchar_t bienvenido[sizeBienvenida];
    swprintf(bienvenido, sizeBienvenida, formato, config.nombre);

    wprintCentro(bienvenido, TITULO_LEN);
    wprintf(L"\n");
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
int obtenerTipoOponente();
void obtenerNombreOponente(char *oponente);

const MenuOrden ejecutarMenuJugar() {
    MenuOrden orden = {VOLVER, L""};
    GoPartida *partidaGuardada = obtenerUltimaPartida();

    if (partidaGuardada && !partidaGuardada->terminada) {
        wprintf(L"Se detectó una partida guardada:\n\n");
        wprintf(L"Fecha: %s\n", obtenerFecha(partidaGuardada->fecha));
        wprintf(L"Tablero: %dx%d\n", partidaGuardada->size, partidaGuardada->size);
        const wchar_t *oponente = strlen(partidaGuardada->oponente) == 0
                                      ? L"máquina"
                                      : strtowcs(partidaGuardada->oponente);
        wprintf(L"Oponente: %ls\n", oponente);
        wprintf(L"Puntaje %s: %d\n", config.nombre, partidaGuardada->puntajeJugador);
        wprintf(L"Puntaje %ls: %d\n\n", oponente, partidaGuardada->puntajeOponente);

        const bool cargar = confirmar(
            L"¿Quieres cargar la partida guardada? Sino será eliminada para siempre", false);
        if (!cargar) {
            borrarUltimaPartida();

            wprintf(L"La partida guardada ha sido eliminada.\n");
            wprintf(L"Presiona ENTER para volver al menú de Jugar.");
            char c = 0;
            while (c != '\n') scanf("%c", &c);

            orden.flag = REPETIR;
            return orden;
        }

        limpiarConsola();
        printTitulo();
        cargarPartida(partidaGuardada);
        jugarPartida(true);

        return orden;
    }

    const int size = obtenerTableroSize();
    if (size == VOLVER) return orden;

    limpiarConsola();
    printTitulo();
    wprintf(L"Tamaño seleccionado: %dx%d\n\n", size, size);

    const int tipo = obtenerTipoOponente();
    if (tipo == REPETIR) {
        orden.flag = REPETIR;
        return orden;
    }

    limpiarConsola();
    printTitulo();
    wprintf(L"Tamaño seleccionado: %dx%d\n", size, size);
    wprintf(L"Tipo de oponente: %ls\n\n", oponentes[tipo]);

    char oponente[NOMBRE_MAX] = "";
    if (tipo == OPONENTE_JUGADOR) {
        obtenerNombreOponente(oponente);
        limpiarConsola();
        printTitulo();
        wprintf(L"Tamaño seleccionado: %dx%d\n", size, size);
        wprintf(L"Tipo de oponente: %ls\n", oponentes[tipo]);
        wprintf(L"Nombre del oponente: %s\n\n", oponente);
    }

    const bool confirmado = confirmar(L"¿Es correcta esta configuración?", false);
    if (!confirmado) {
        orden.flag = REPETIR;
        return orden;
    }

    limpiarConsola();
    printTitulo();
    crearPartida(size, oponente);
    jugarPartida(false);

    return orden;
}

const MenuOrden ejecutarMenuReglas() {
    FILE *archivoReglas = fopen(rutaReglas, "r");
    fseek(archivoReglas, 0, SEEK_END);
    const long max = ftell(archivoReglas) + 1;
    fseek(archivoReglas, 0, SEEK_SET);

    char reglas[max];
    fread(reglas, 1, max, archivoReglas);
    fclose(archivoReglas);
    reglas[max - 1] = '\0';

    printCentro("-== REGLAS DE GO ==-\n", TITULO_LEN);
    wprintConLineLimit(strtowcs(reglas), TITULO_LEN);

    wprintf(L"\n\nPresiona ENTER para volver al menú principal.");
    char c = 0;
    while (c != '\n') scanf("%c", &c);

    const MenuOrden orden = {VOLVER, L""};
    return orden;
}

int obtenerConfig();

const MenuOrden ejecutarMenuConfig() {
    MenuOrden orden = {VOLVER, L""};
    const int menuConfig = obtenerConfig();
    if (menuConfig == VOLVER) return orden;

    if (menuConfig == CONFIG_NOMBRE) {
        orden.flag = REPETIR;
        char *antiguo = strdup(config.nombre);
        strempty(config.nombre);
        obtenerNombre();
        swprintf(orden.mensaje, MENSAJE_ORDEN_LEN, L"Nombre de usuario cambiado de \"%s\" a \"%s\"", antiguo, config.nombre);
        free(antiguo);
        return orden;
    }

    if (menuConfig == CONFIG_BORRAR_PARTIDAS) {
        orden.flag = REPETIR;
        const bool confirmado = confirmar(wcslower(configs[menuConfig]), true);
        if (confirmado) borrarTodasPartidas();
        swprintf(orden.mensaje, MENSAJE_ORDEN_LEN, L"%s",
                 confirmado ? "Las partidas guardadas han sido borradas" : "Cancelado");
        return orden;
    }

    if (menuConfig == CONFIG_BORRAR_TODO) {
        orden.flag = REPETIR;
        const bool confirmado = confirmar(wcslower(configs[menuConfig]), true);
        if (confirmado) {
            borrarTodasPartidas();
            borrarConfig();
        }
        swprintf(orden.mensaje, MENSAJE_ORDEN_LEN, L"%s",
                 confirmado ? "Todas los datos guardados han sido borradas" : "Cancelado");
        return orden;
    }

    return orden;
}

int compararPartidas(const void *a, const void *b);
void printStats(const TodasGoPartidas todasPartidas, int page);

const MenuOrden ejecutarMenuStats() {
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

    free(todasPartidas.partidas);

    const MenuOrden orden = {VOLVER, L""};
    return orden;
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

    wprintf(L"%s", strrepeat(' ', idPad + 2));
    for (int i = 0; i < STATS; i++)
        wprintf(wcspadright(stats[i], STATS_PAD, ' '));
    wprintf(L"\n");

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
        wprintf(L"%d. %dx%d\n", i + 1, size, size);
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

int obtenerTipoOponente() {
    for (int i = 0; i < OPONENTES; i++) {
        wprintf(L"%d. %ls\n", i + 1, oponentes[i]);
    }
    wprintf(L"%d. Volver al menú de Jugar\n\n", OPONENTES + 1);

    int tipo;
    wprintf(L"Selecciona el tipo oponente: ");

    while (!getInt(&tipo) || tipo < 1 || tipo > OPONENTES + 1) {
        wprintf(L"Opción inválida. Intenta de nuevo: ");
    }

    if (tipo == OPONENTES + 1) return REPETIR;
    return tipo - 1;
}

void obtenerNombreOponente(char *oponente) {
    wprintf(L"¿Cuál es el nombre del oponente? Máximo %d caracteres (alfanuméricos y guion bajo).\n", NOMBRE_MAX - 1);
    strget(oponente, NOMBRE_MAX);

    while (!validarNombre(oponente)) {
        wprintf(L"Nombre inválido. Intenta de nuevo: ");
        strget(oponente, NOMBRE_MAX);
    }
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
