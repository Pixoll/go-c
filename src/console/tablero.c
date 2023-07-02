#include "../common/tablero.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/datos.h"
#include "../common/util.h"
#include "util.h"

#define VERSUS_LEN 2 * (NOMBRE_MAX - 1) + 5
#define PUNTAJE_LEN 2 * (INT_STR_MAX - 1) + 5
#define TURNO_LEN 2 * (NOMBRE_MAX - 1) + 13
#define SUSPENSO_LEN TITULO_LEN / 2 - 3

enum PARTIDA_FLAG {
    TERMINAR = -2,
    GUARDAR,
};

const wchar_t celdas[CELDAS] = {'X', 'O', '+', L'¦', '?', '?', '?', '?'};

void printTablero() {
    const int size = partida.size;
    const int tableroLen = size * 2 + 3;
    wchar_t tablero[size][tableroLen];

    for (int i = 0; i < size; i++) {
        swprintf(tablero[i], tableroLen, L"%2d ", i + 1);
        for (int j = 0; j < size; j++) {
            const wchar_t celda = celdas[partida.tablero[i][j]];
            const int lenCopia = wcslen(tablero[i]) + 1;
            wchar_t copia[lenCopia];
            wcsncpy(copia, tablero[i], lenCopia);
            swprintf(tablero[i], tableroLen, L"%ls%lc-", copia, celda);
        }
        tablero[i][tableroLen - 1] = '\0';
    }

    wchar_t versus[VERSUS_LEN];
    swprintf(versus, VERSUS_LEN, L"%s vs. %ls\n", config.nombre, esMaquina() ? L"Máquina" : strtowcs(partida.oponente));
    wprintCentro(versus, TITULO_LEN);

    char capturas[PUNTAJE_LEN];
    snprintf(capturas, PUNTAJE_LEN, "%*d | %-*d\n",
             INT_STR_MAX - 1,
             partida.puntajeJugador,
             INT_STR_MAX - 1,
             partida.puntajeOponente);
    printCentro("Capturas", TITULO_LEN);
    printCentro(capturas, TITULO_LEN);

    for (int i = size - 1; i >= 0; i--)
        wprintCentro(tablero[i], TITULO_LEN - 2);

    char columnas[tableroLen - 1];
    for (int i = 0; i < tableroLen - 2; i++)
        columnas[i] = i == 0 || !par(i) ? ' ' : 'A' + i / 2 - 1;
    columnas[tableroLen - 2] = '\0';
    printCentro(columnas, TITULO_LEN);

    wprintf(L"\n");
}

bool obtenerCelda(int *px, int *py, bool reintentar);
bool confirmarTerminoPartida(const wchar_t *oponente);

void jugarPartida(bool cargada) {
    printTablero();
    const int size = partida.size;
    const wchar_t *nombreOponente = esMaquina() ? L"Máquina" : strtowcs(partida.oponente);

    char anterior[size][size];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            anterior[i][j] = partida.anterior[i][j];

    int ultimoX = -1, ultimoY = -1;
    bool terminar = false, guardar = false, primerTurno = !cargada;
    while (!terminar) {
        const celda_t jugador = par(partida.turno) ? CELDA_NEGRA : CELDA_BLANCA;
        const celda_t oponente = jugador == CELDA_NEGRA ? CELDA_BLANCA : CELDA_NEGRA;

        wchar_t turnoTexto[TURNO_LEN];
        swprintf(turnoTexto, TURNO_LEN, L"Turno de %ls (%lc)\n",
                 par(partida.turno) ? strtowcs(config.nombre) : nombreOponente,
                 celdas[jugador]);
        wprintCentro(turnoTexto, TITULO_LEN);

        if (ultimoX != -1 && ultimoY != -1) {
            wprintf(L"%ls (%lc) jugó %c%d\n",
                    par(partida.turno) ? nombreOponente : strtowcs(config.nombre),
                    celdas[oponente],
                    'A' + ultimoY, ultimoX + 1);
        }

        int x, y;
        if (esMaquina() && !par(partida.turno)) {
            jugarMaquina(&x, &y);
            wprintf(L"%*s", SUSPENSO_LEN, "");
            // suspenso o.o completamente innecesario xD
            wprintf(L".");
            esperar(1);
            wprintf(L" .");
            esperar(1);
            wprintf(L" .");
            esperar(1);
        } else {
            while (true) {
                bool reintentar = false;
                while (!obtenerCelda(&x, &y, reintentar))
                    reintentar = true;

                if (x == TERMINAR) {
                    terminar = true;
                    break;
                }

                if (x == GUARDAR) {
                    const bool confirmado = confirmar(L"guardar la partida y volver al menú principal", true);
                    if (!confirmado) continue;
                    guardar = true;
                    break;
                }

                if (estaOcupada(x, y)) {
                    wprintf(L"! Esa casilla ya esta ocupada.\n");
                    continue;
                }

                if (ko(x, y, jugador, oponente)) {
                    wprintf(L"! Colocar una ficha acá causaría un KO. Espera al menos un turno para poner una ficha aquí.\n");
                    continue;
                }

                if (suicidio(x, y, jugador, oponente)) {
                    wprintf(L"! Colocar una ficha acá sería SUICIDIO.\n");
                    continue;
                }

                break;
            }
        }

        if (guardar) break;

        const bool terminoSolicitado = terminar;
        if (!terminar) {
            marcarOcupada(x, y);
            partida.tablero[x][y] = jugador;

            const bool realizaronCapturas = capturas(oponente, jugador);
            if (realizaronCapturas) eliminarCapturadas();
        } else {
            if (primerTurno) break;
            terminar = confirmarTerminoPartida(nombreOponente);
        }

        if (terminar) break;

        limpiarConsola();
        printTitulo();
        printTablero();

        if (terminoSolicitado) continue;
        partida.turno++;
        primerTurno = false;
        ultimoX = x;
        ultimoY = y;

        if (partida.turno - 2 >= 0) {
            for (int i = 0; i < partida.size; i++)
                for (int j = 0; j < partida.size; j++)
                    partida.anterior[i][j] = anterior[i][j];
        }
        if (partida.turno - 1 >= 0) {
            for (int i = 0; i < partida.size; i++)
                for (int j = 0; j < partida.size; j++)
                    anterior[i][j] = partida.tablero[i][j];
        }
    }

    if (primerTurno) return;

    partida.terminada = terminar;
    partida.fecha = now();
    guardarPartida(partida, cargada);

    if (!terminar)
        wprintf(L"\nLa partida ha sido guardada y la podrás acceder nuevamente en el menú de Jugar.");

    wprintf(L"\nPresiona ENTER para volver al menú principal.");
    char c = 0;
    while (c != '\n') scanf("%c", &c);
}

bool obtenerCelda(int *px, int *py, bool reintentar) {
    if (!reintentar)
        wprintf(L"Ingresa la casilla (0 para terminar, 1 para guardar e ir al menú principal): ");
    else
        wprintf(L"Casilla inválida. Intenta de nuevo: ");

    const int size = partida.size;
    char col;
    int fila;
    scanf("%c", &col);
    if (!getInt(&fila)) return false;

    if (fila == 0 && (col == '0' || col == '1')) {
        *px = col - '0' - 2;
        return true;
    }

    if (fila < 1 || fila > size)
        return false;

    if (col >= 'a' && col <= 'a' + size - 1)
        col += 'A' - 'a';

    if (col < 'A' || col > 'A' + size - 1)
        return false;

    *py = col - 'A';
    *px = fila - 1;
    return true;
}

bool confirmarTerminoPartida(const wchar_t *oponente) {
    const int size = partida.size;
    const int puntajeJugador = partida.puntajeJugador;
    const int puntajeOponente = partida.puntajeOponente;
    char antiguo[size][size];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            antiguo[i][j] = partida.tablero[i][j];

    // Chequeo de capturas
    const bool realizaronCapturas1 = capturas(CELDA_NEGRA, CELDA_BLANCA);
    const bool realizaronCapturas2 = capturas(CELDA_BLANCA, CELDA_NEGRA);
    if (realizaronCapturas1 || realizaronCapturas2) eliminarCapturadas();

    limpiarConsola();
    printTitulo();
    printTablero();

    puntajePorCantidad();
    puntajePorArea();

    wprintf(L"Puntaje %s: %d\n", config.nombre, partida.puntajeJugador);
    wprintf(L"Puntaje %ls: %d\n\n", oponente, partida.puntajeOponente);

    wprintf(L"Para el puntaje total se considera el número de fichas en el tablero, las capturas y las áreas.\n");
    const bool confirmado = confirmar(L"¿Están de acuerdo con los resultados?", false);
    if (!confirmado) {
        partida.puntajeJugador = puntajeJugador;
        partida.puntajeOponente = puntajeOponente;
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++)
                partida.tablero[i][j] = antiguo[i][j];
    }

    return confirmado;
}
