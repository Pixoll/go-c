#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datos.h"
#include "util.h"

#define VERSUS_LEN 2 * (NOMBRE_MAX - 1) + 5
#define TURNO_LEN 2 * (NOMBRE_MAX - 1) + 10

#define CELDAS 9
enum CELDA {
    CELDA_BORDE,
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

const wchar_t celdas[CELDAS] = {'?', 'X', 'O', '+', L'¦', '?', '?', '?', '?'};
const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};

GoPartida partida;
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
    partida.turnoNegras = true;
    for (int i = 1; i <= size; i++)
        for (int j = 1; j <= size; j++)
            ocupadas[i][j] = false;

    for (int i = 1; i <= size; i++) {
        partida.tablero[1][i] = CELDA_EMPTY_HOR;
        partida.tablero[size][i] = CELDA_EMPTY_HOR;
        if (i == 1 || i == size) continue;
        for (int j = 1; j <= size; j++)
            partida.tablero[i][j] = CELDA_EMPTY_VERT;
    }
}

void cargarPartida(GoPartida *guardada) {
    partida.size = guardada->size;
    snprintf(partida.oponente, NOMBRE_MAX, "%s", guardada->oponente);
    partida.puntajeJugador = guardada->puntajeJugador;
    partida.puntajeOponente = guardada->puntajeOponente;
    partida.turnoNegras = guardada->turnoNegras;
    for (int i = 0; i < TABLERO_MAX; i++)
        for (int j = 0; j < TABLERO_MAX; j++)
            partida.tablero[i][j] = guardada->tablero[i][j];
}

void printTablero() {
    const int size = partida.size;
    const int tableroLen = size * 2 + 3;
    wchar_t tablero[size][tableroLen];

    for (int i = 1; i <= size; i++) {
        swprintf(tablero[i - 1], tableroLen, L"%2d ", i);
        for (int j = 1; j <= size; j++) {
            const wchar_t celda = celdas[partida.tablero[i][j]];
            wchar_t *copia = wcsdup(tablero[i - 1]);
            swprintf(tablero[i - 1], tableroLen, L"%ls%lc-", copia, celda);
            free(copia);
        }
        tablero[i - 1][tableroLen - 1] = '\0';
    }

    wchar_t versus[VERSUS_LEN];
    swprintf(versus, VERSUS_LEN, L"%s vs. %ls\n", config.nombre, esMaquina() ? L"Máquina" : strtowcs(partida.oponente));
    wprintCentro(versus, TITULO_LEN);

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

void jugarPartida(bool cargada) {
    printTablero();
    const int size = partida.size;
    int turno = 0;
    const wchar_t *oponente = esMaquina() ? L"máquina" : strtowcs(partida.oponente);

    bool terminar = false, guardar = false;
    while (!terminar) {
        wchar_t turnoTexto[TURNO_LEN];
        swprintf(turnoTexto, TURNO_LEN, L"Turno de %ls\n", partida.turnoNegras ? strtowcs(config.nombre) : oponente);
        wprintCentro(turnoTexto, TITULO_LEN);

        int x, y;
        if (esMaquina() && !partida.turnoNegras) {
            jugarMaquina(&x, &y);
            wprintf(wcsrepeat(L' ', TITULO_LEN / 2 - 3));
            // suspenso o.o completamente innecesario xD
            esperar(1);
            wprintf(L".");
            esperar(1);
            wprintf(L" .");
            esperar(1);
            wprintf(L" .");
            esperar(1);
        } else {
            do {
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

                if (ocupadas[x][y] == true)
                    wprintf(L"¡Esa casilla ya esta ocupada!\n");
            } while (ocupadas[x][y] == true);
        }

        if (guardar) break;

        ocupadas[x][y] = true;
        partida.tablero[x][y] = partida.turnoNegras ? CELDA_NEGRA : CELDA_BLANCA;

        // Chequeo de capturas
        capturas(CELDA_NEGRA, CELDA_BLANCA);
        capturas(CELDA_BLANCA, CELDA_NEGRA);
        eliminarCapturadas();

        limpiarConsola();
        printTitulo();
        printTablero();

        partida.turnoNegras = !partida.turnoNegras;
        turno++;
    }

    partida.terminada = terminar;
    partida.fecha = now();
    if (terminar) {
        puntajePorCantidad();
        puntajePorArea();

        wprintf(L"Puntaje %s: %d\n", config.nombre, partida.puntajeJugador);
        wprintf(L"Puntaje %ls: %d\n\n", oponente, partida.puntajeOponente);
    }

    guardarPartida(partida, cargada);

    if (!terminar)
        wprintf(L"La partida ha sido guardada y la podrás acceder nuevamente en el menú de Jugar.\n");

    wprintf(L"Presiona ENTER para volver al menú principal.");
    char c = 0;
    while (c != '\n') scanf("%c", &c);
}

bool obtenerCelda(int *px, int *py, bool reintentar) {
    if (!reintentar)
        wprintf(L"Ingresa la casilla (0 para terminar, 1 para guardar e ir al menú principal): ");
    else
        wprintf(L"Casilla inválida. Intenta de nuevo: ");

    const int size = partida.size;
    char col, fila[3];
    scanf("%c", &col);
    strget(fila, 3);

    if (col == '0' || col == '1') {
        *px = col - '0' - 2;
        return true;
    }

    if ((col < 'a' || col > 'a' + size - 1) && (col < 'A' || col > 'A' + size - 1))
        return false;

    const int f = atoi(fila);
    if (f < 1 || f > size)
        return false;

    *py = col >= 'a' && col <= 'a' + size - 1 ? col - 'a' + 1 : col - 'A' + 1;
    *px = f;
    return true;
}

// pone una ficha random
void jugarMaquina(int *px, int *py) {
    const int size = partida.size + 1;
    int x, y;
    do {
        x = rand() % size;
        y = rand() % size;
    } while (ocupadas[x][y] == true);
    *px = x;
    *py = y;
}

void puntajePorCantidad() {
    for (int i = 1; i <= partida.size; i++) {
        for (int j = 1; j <= partida.size; j++) {
            const char celda = partida.tablero[i][j];
            if (celda == CELDA_NEGRA) partida.puntajeJugador++;
            if (celda == CELDA_BLANCA) partida.puntajeOponente++;
        }
    }
}

void expandir();

// función que pasara por todos los puntos y buscara areas vacías
void puntajePorArea() {
    int c = 0;
    for (int i = 1; i <= partida.size; i++) {
        for (int j = 1; j <= partida.size; j++) {
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

    int c = 1, total = 1;
    while (c > 0) {
        c = 0;
        for (int i = 1; i <= partida.size; i++) {
            for (int j = 1; j <= partida.size; j++) {
                if (partida.tablero[i][j] != CELDA_EMPTY) continue;
                for (int r = 0; r < 4; r++) {
                    const int x = i + dx[r];
                    const int y = j + dy[r];
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

    for (int i = 1; i <= partida.size; i++) {
        for (int j = 1; j <= partida.size; j++) {
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

    for (int i = 1; i <= partida.size; i++) {
        for (int j = 1; j <= partida.size; j++) {
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

        // verifica si las fichas vecinas son validas
        if (nx >= 1 && nx <= partida.size && ny >= 1 && ny <= partida.size) {
            // verifica si hay espacios en blanco en las fichas vecinas
            if (partida.tablero[nx][ny] == CELDA_EMPTY_HOR || partida.tablero[nx][ny] == CELDA_EMPTY_VERT) {
                *tieneLibertades = true;
            } else if (partida.tablero[nx][ny] == celdaJugador && !visitado[nx][ny]) {
                DFS(nx, ny, celdaJugador, celdaOponente, visitado, tieneLibertades, grupo);
            }
        }
    }
}

// analiza el tablero y los grupos de fichas que se forman, usa dfs para recorrer los grupos y ve si están o no capturados
void capturas(celda_t celdaJugador, celda_t celdaOponente) {
    bool visitado[TABLERO_MAX][TABLERO_MAX];
    for (int i = 1; i <= partida.size; i++)
        for (int j = 1; j <= partida.size; j++)
            visitado[i][j] = false;

    for (int posX = 1; posX <= partida.size; posX++) {
        for (int posY = 1; posY <= partida.size; posY++) {
            if (partida.tablero[posX][posY] != celdaJugador || visitado[posX][posY]) continue;

            bool tieneLibertades = false;
            int grupo[TABLERO_MAX][TABLERO_MAX];
            for (int i = 1; i <= partida.size; i++)
                for (int j = 1; j <= partida.size; j++)
                    grupo[i][j] = 0;

            DFS(posX, posY, celdaJugador, celdaOponente, visitado, &tieneLibertades, grupo);
            if (tieneLibertades) continue;

            for (int i = 1; i <= partida.size; i++) {
                for (int j = 1; j <= partida.size; j++) {
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

// saca las fichas capturadas y otorgar puntaje correspondiente
void eliminarCapturadas() {
    for (int posX = 1; posX <= partida.size; posX++) {
        for (int posY = 1; posY <= partida.size; posY++) {
            const celda_t celda = partida.tablero[posX][posY];
            if (celda != CELDA_BLANCA_CAPT && celda != CELDA_NEGRA_CAPT) continue;

            const celda_t reemplazo = posX == 1 || posX == partida.size ? CELDA_EMPTY_HOR : CELDA_EMPTY_VERT;
            partida.tablero[posX][posY] = reemplazo;
            if (celda == CELDA_BLANCA_CAPT)
                partida.puntajeJugador++;
            else
                partida.puntajeOponente++;
        }
    }
}
