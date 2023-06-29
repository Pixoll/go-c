#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datos.h"
#include "util.h"

#define VERSUS_LEN 2 * (NOMBRE_MAX - 1) + 5
#define PUNTAJE_LEN 2 * (INT_STR_MAX - 1) + 5
#define TURNO_LEN 2 * (NOMBRE_MAX - 1) + 10
#define SUSPENSO_LEN TITULO_LEN / 2 - 3

#define CELDAS 8
enum CELDA {
    CELDA_NEGRA,
    CELDA_BLANCA,
    CELDA_EMPTY_HOR,
    CELDA_EMPTY_VERT,
    CELDA_NEGRA_CAPT,
    CELDA_BLANCA_CAPT,
    CELDA_EMPTY,
    CELDA_ANALIZADA,
};
typedef enum CELDA celda_t;

enum PARTIDA_FLAG {
    TERMINAR = -2,
    GUARDAR,
};

const wchar_t celdas[CELDAS] = {'X', 'O', '+', L'¦', '?', '?', '?', '?'};
const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};

Partida partida;
bool ocupadas[TABLERO_MAX][TABLERO_MAX];

bool esMaquina() {
    return strlen(partida.oponente) == 0;
}

void crearPartida(int size, char *oponente) {
    // para la máquina
    srand(now());

    partida.size = size;
    snprintf(partida.oponente, NOMBRE_MAX, "%s", oponente);
    partida.puntajeJugador = 0;
    partida.puntajeOponente = 0;
    partida.turno = 0;
    partida.fecha = 0;
    partida.terminada = false;

    for (int i = 0; i < 2; i++) {
        partida.ultimasCapturas[i].turno = -1;
        partida.ultimasCapturas[i].x = -1;
        partida.ultimasCapturas[i].y = -1;
    }

    for (int i = 0; i < size; i++) {
        partida.tablero[0][i] = CELDA_EMPTY_HOR;
        partida.tablero[size - 1][i] = CELDA_EMPTY_HOR;
        if (i == 0 || i == size - 1) continue;
        for (int j = 0; j < size; j++)
            partida.tablero[i][j] = CELDA_EMPTY_VERT;
    }

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            ocupadas[i][j] = false;
}

void cargarPartida(Partida *guardada) {
    partida.size = guardada->size;
    snprintf(partida.oponente, NOMBRE_MAX, "%s", guardada->oponente);
    partida.puntajeJugador = guardada->puntajeJugador;
    partida.puntajeOponente = guardada->puntajeOponente;
    partida.turno = guardada->turno;
    for (int i = 0; i < TABLERO_MAX; i++)
        for (int j = 0; j < TABLERO_MAX; j++)
            partida.tablero[i][j] = guardada->tablero[i][j];
}

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

void puntajePorCantidad();
void puntajePorArea();
void capturas(celda_t celdaJugador, celda_t celdaOponente);
void eliminarCapturadas();
void jugarMaquina(int *px, int *py);
bool obtenerCelda(int *px, int *py, bool reintentar);
bool confirmarTerminoPartida(const wchar_t *oponente);
bool ko(int x, int y);
bool suicidio(int x, int y, celda_t celdaJugador, celda_t celdaOponente);

void jugarPartida(bool cargada) {
    printTablero();
    const wchar_t *nombreOponente = esMaquina() ? L"máquina" : strtowcs(partida.oponente);

    bool terminar = false, guardar = false, primerTurno = !cargada;
    while (!terminar) {
        wchar_t turnoTexto[TURNO_LEN];
        swprintf(turnoTexto, TURNO_LEN, L"Turno de %ls\n", par(partida.turno) ? strtowcs(config.nombre) : nombreOponente);
        wprintCentro(turnoTexto, TITULO_LEN);

        const celda_t jugador = par(partida.turno) ? CELDA_NEGRA : CELDA_BLANCA;
        const celda_t oponente = jugador == CELDA_NEGRA ? CELDA_BLANCA : CELDA_NEGRA;

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

                if (ocupadas[x][y] == true) {
                    wprintf(L"! Esa casilla ya esta ocupada.\n");
                    continue;
                }

                if (suicidio(x, y, jugador, oponente)) {
                    wprintf(L"! Colocar una ficha acá sería SUICIDIO.\n");
                    continue;
                }

                // TODO: regla de suicidio debe estar bien implementada antes de usar el ko
                // if (ko(x, y)) {
                //     wprintf(L"! Colocar una ficha acá causaría un KO. Espera al menos un turno para poner una ficha aquí.\n");
                //     continue;
                // }

                break;
            }
        }

        if (guardar) break;

        const bool terminoSolicitado = terminar;
        if (!terminar) {
            ocupadas[x][y] = true;
            partida.tablero[x][y] = jugador;

            capturas(oponente, jugador);
            eliminarCapturadas();
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

bool estaEnTablero(int x, int y) {
    return x >= 0 && x <= partida.size - 1 && y >= 0 && y <= partida.size - 1;
}

// pone una ficha random
void jugarMaquina(int *px, int *py) {
    const int size = partida.size;
    int x, y;
    do {
        x = rand() % size;
        y = rand() % size;
    } while (!estaEnTablero(x, y) || ocupadas[x][y] == true);
    *px = x;
    *py = y;
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
    capturas(CELDA_NEGRA, CELDA_BLANCA);
    capturas(CELDA_BLANCA, CELDA_NEGRA);
    eliminarCapturadas();

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

void puntajePorCantidad() {
    for (int i = 0; i < partida.size; i++) {
        for (int j = 0; j < partida.size; j++) {
            const char celda = partida.tablero[i][j];
            if (celda == CELDA_NEGRA) partida.puntajeJugador++;
            if (celda == CELDA_BLANCA) partida.puntajeOponente++;
        }
    }
}

void expandir();

// función que pasara por todos los puntos y buscara areas vacías
void puntajePorArea() {
    for (int i = 0; i < partida.size; i++) {
        for (int j = 0; j < partida.size; j++) {
            const celda_t celda = partida.tablero[i][j];
            if (celda != CELDA_EMPTY_HOR && celda != CELDA_EMPTY_VERT) continue;
            partida.tablero[i][j] = CELDA_EMPTY;
            expandir();
        }
    }
}

void verificarArea(int total);

void expandir() {
    // en base al area vacía inicial busca todas las areas vacías contiguas y las caracteriza con CELDA_EMPTY
    // contador que nos servirá para saber cuando se dejan de encontrar areas vacías contiguas
    // cantidad de espacios vacíos encerrados dentro de un area
    // ciclo que expande el area inicial cambiando las areas vacías contiguas por CELDA_EMPTY,
    // se repite siempre que se haya expandido una ultima vez (c>0)

    const int size = partida.size;
    int c = 1, total = 1;
    while (c > 0) {
        c = 0;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (partida.tablero[i][j] != CELDA_EMPTY) continue;
                for (int r = 0; r < 4; r++) {
                    const int x = i + dx[r];
                    const int y = j + dy[r];
                    // No salir del tablero
                    if (!estaEnTablero(x, y)) continue;

                    const celda_t celda = partida.tablero[x][y];
                    if (celda != CELDA_EMPTY_HOR && celda != CELDA_EMPTY_VERT) continue;
                    partida.tablero[x][y] = CELDA_EMPTY;
                    c++;
                    total++;
                }
            }
        }
    }

    verificarArea(total);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (partida.tablero[i][j] != CELDA_EMPTY) continue;
            partida.tablero[i][j] = CELDA_ANALIZADA;
        }
    }
}

bool tieneCeldaAlrededor(int x, int y, celda_t tipo);

// analizamos el area de CELDA_EMPTY verificando si solo la rodea un tipo de ficha o no
void verificarArea(int total) {
    int negras = 0;
    int blancas = 0;
    int otro = 0;

    for (int i = 0; i < partida.size; i++) {
        for (int j = 0; j < partida.size; j++) {
            if (partida.tablero[i][j] != CELDA_EMPTY) continue;
            if (tieneCeldaAlrededor(i, j, CELDA_NEGRA))
                negras++;
            if (tieneCeldaAlrededor(i, j, CELDA_BLANCA))
                blancas++;
            if (!tieneCeldaAlrededor(i, j, CELDA_NEGRA) && !tieneCeldaAlrededor(i, j, CELDA_BLANCA))
                otro++;
        }
    }

    if (negras != 0 && blancas == 0 && otro < negras * 2) {
        partida.puntajeJugador += total;
    } else if (blancas != 0 && negras == 0 && otro < blancas * 2) {
        partida.puntajeOponente += total;
    }

    // de no cumplirse ninguna condición anterior se asume que el area es neutra y no se le otorgara puntaje a ningún jugador
    // mas a delante se pueden implementar mas condiciones de area neutra haciendo las condiciones del juego mas realista
}

bool tieneCeldaAlrededor(int x, int y, celda_t tipo) {
    for (int r = 0; r < 4; r++) {
        const int nx = x + dx[r];
        const int ny = y + dy[r];
        // No salir del tablero
        if (!estaEnTablero(nx, ny)) continue;
        if (partida.tablero[nx][ny] == tipo) return true;
    }
    return false;
}

// busca y recorre los grupos de fichas, los marca como que tienen o no libertades
void DFS(int posX, int posY, celda_t celdaJugador, celda_t celdaOponente, bool visitado[TABLERO_MAX][TABLERO_MAX], bool *tieneLibertades, int grupo[TABLERO_MAX][TABLERO_MAX]) {
    visitado[posX][posY] = true;
    grupo[posX][posY] = 1;

    for (int i = 0; i < 4; i++) {
        const int nx = posX + dx[i];
        const int ny = posY + dy[i];
        // No salir del tablero
        if (!estaEnTablero(nx, ny)) continue;

        // verifica si hay espacios en blanco en las fichas vecinas
        if (partida.tablero[nx][ny] == CELDA_EMPTY_HOR || partida.tablero[nx][ny] == CELDA_EMPTY_VERT) {
            *tieneLibertades = true;
        } else if (partida.tablero[nx][ny] == celdaJugador && !visitado[nx][ny]) {
            DFS(nx, ny, celdaJugador, celdaOponente, visitado, tieneLibertades, grupo);
        }
    }
}

// analiza el tablero y los grupos de fichas que se forman, usa dfs para recorrer los grupos y ve si están o no capturados
void capturas(celda_t celdaJugador, celda_t celdaOponente) {
    bool visitado[TABLERO_MAX][TABLERO_MAX];
    for (int i = 0; i < partida.size; i++)
        for (int j = 0; j < partida.size; j++)
            visitado[i][j] = false;

    for (int posX = 0; posX < partida.size; posX++) {
        for (int posY = 0; posY < partida.size; posY++) {
            if (partida.tablero[posX][posY] != celdaJugador || visitado[posX][posY]) continue;

            bool tieneLibertades = false;
            int grupo[TABLERO_MAX][TABLERO_MAX];
            for (int i = 0; i < partida.size; i++)
                for (int j = 0; j < partida.size; j++)
                    grupo[i][j] = 0;

            DFS(posX, posY, celdaJugador, celdaOponente, visitado, &tieneLibertades, grupo);
            if (tieneLibertades) continue;

            for (int i = 0; i < partida.size; i++) {
                for (int j = 0; j < partida.size; j++) {
                    if (grupo[i][j] != 1) continue;
                    // marca la ficha como capturada
                    partida.tablero[i][j] = celdaOponente == CELDA_NEGRA ? CELDA_BLANCA_CAPT : CELDA_NEGRA_CAPT;
                    // actualiza el estado de 'ocupado'
                    ocupadas[i][j] = false;
                }
            }
        }
    }
}

void guardarCaptura(int x, int y) {
    swap(&partida.ultimasCapturas[0].turno, &partida.ultimasCapturas[1].turno);
    swap(&partida.ultimasCapturas[0].x, &partida.ultimasCapturas[1].x);
    swap(&partida.ultimasCapturas[0].y, &partida.ultimasCapturas[1].y);

    partida.ultimasCapturas[1].turno = partida.turno;
    partida.ultimasCapturas[1].x = x;
    partida.ultimasCapturas[1].y = y;
}

// saca las fichas capturadas y otorgar puntaje correspondiente
void eliminarCapturadas() {
    for (int posX = 0; posX < partida.size; posX++) {
        for (int posY = 0; posY < partida.size; posY++) {
            const celda_t celda = partida.tablero[posX][posY];
            if (celda != CELDA_BLANCA_CAPT && celda != CELDA_NEGRA_CAPT) continue;

            const celda_t reemplazo = posX == 0 || posX == partida.size - 1 ? CELDA_EMPTY_HOR : CELDA_EMPTY_VERT;
            partida.tablero[posX][posY] = reemplazo;
            if (celda == CELDA_BLANCA_CAPT)
                partida.puntajeJugador++;
            else
                partida.puntajeOponente++;

            guardarCaptura(posX, posY);
        }
    }
}

bool ko(int x, int y) {
    const int turno = partida.turno;
    const Captura captura1 = partida.ultimasCapturas[0];
    const Captura captura2 = partida.ultimasCapturas[1];
    if (captura1.turno == -1 || captura2.turno == -1 || captura1.turno != turno - 2 || captura2.turno != turno - 1)
        return false;

    const int size = partida.size;
    char antiguo[size][size];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            antiguo[i][j] = partida.tablero[i][j];

    capturas(CELDA_NEGRA, CELDA_BLANCA);
    capturas(CELDA_BLANCA, CELDA_NEGRA);

    const celda_t capturada = partida.tablero[captura1.x][captura1.y];
    const bool esKO = capturada == CELDA_NEGRA_CAPT || capturada == CELDA_BLANCA_CAPT;

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            partida.tablero[i][j] = antiguo[i][j];

    return esKO;
}

bool suicidio(int x, int y, celda_t celdaJugador, celda_t celdaOponente) {
    bool tieneLibertades = false;
    bool visitado[TABLERO_MAX][TABLERO_MAX];
    int grupo[TABLERO_MAX][TABLERO_MAX];
    for (int i = 0; i < partida.size; i++)
        for (int j = 0; j < partida.size; j++)
            visitado[i][j] = false;
    for (int i = 0; i < partida.size; i++)
        for (int j = 0; j < partida.size; j++)
            grupo[i][j] = 0;

    const celda_t previo = partida.tablero[x][y];
    partida.tablero[x][y] = celdaJugador;
    DFS(x, y, celdaJugador, celdaOponente, visitado, &tieneLibertades, grupo);
    partida.tablero[x][y] = previo;
    return !tieneLibertades;
}
