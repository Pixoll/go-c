#include "tablero.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "datos.h"
#include "util.h"

const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};

Partida partida;
bool ocupadas[TABLERO_MAX][TABLERO_MAX];

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

    for (int i = 0; i < size; i++) {
        partida.tablero[0][i] = CELDA_EMPTY_HOR;
        partida.anterior[0][i] = CELDA_EMPTY_HOR;
        partida.tablero[size - 1][i] = CELDA_EMPTY_HOR;
        partida.anterior[size - 1][i] = CELDA_EMPTY_HOR;
        if (i == 0 || i == size - 1) continue;
        for (int j = 0; j < size; j++) {
            partida.tablero[i][j] = CELDA_EMPTY_VERT;
            partida.anterior[i][j] = CELDA_EMPTY_VERT;
        }
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
    for (int i = 0; i < TABLERO_MAX; i++)
        for (int j = 0; j < TABLERO_MAX; j++)
            partida.anterior[i][j] = guardada->anterior[i][j];
}

bool esMaquina() {
    return strlen(partida.oponente) == 0;
}

bool estaEnTablero(int x, int y) {
    return x >= 0 && x <= partida.size - 1 && y >= 0 && y <= partida.size - 1;
}

bool estaOcupada(int x, int y) {
    return ocupadas[x][y];
}

void marcarOcupada(int x, int y) {
    ocupadas[x][y] = true;
}

bool ko(int x, int y, celda_t celdaJugador, celda_t celdaOponente);
bool suicidio(int x, int y, celda_t celdaJugador, celda_t celdaOponente);

// pone una ficha random
void jugarMaquina(int *px, int *py) {
    const int size = partida.size;
    int x, y;
    do {
        x = rand() % size;
        y = rand() % size;
    } while (!estaEnTablero(x, y) || ocupadas[x][y] == true || ko(x, y, CELDA_BLANCA, CELDA_NEGRA) || suicidio(x, y, CELDA_BLANCA, CELDA_NEGRA));
    *px = x;
    *py = y;
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
void DFS(int posX, int posY, celda_t celdaJugador, celda_t celdaOponente, bool visitado[TABLERO_MAX][TABLERO_MAX], bool *tieneLibertades, bool grupo[TABLERO_MAX][TABLERO_MAX]) {
    visitado[posX][posY] = true;
    grupo[posX][posY] = true;

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
bool capturas(celda_t celdaJugador, celda_t celdaOponente) {
    bool realizaronCapturas = false;
    bool visitado[TABLERO_MAX][TABLERO_MAX];
    for (int i = 0; i < partida.size; i++)
        for (int j = 0; j < partida.size; j++)
            visitado[i][j] = false;

    for (int posX = 0; posX < partida.size; posX++) {
        for (int posY = 0; posY < partida.size; posY++) {
            if (partida.tablero[posX][posY] != celdaJugador || visitado[posX][posY]) continue;

            bool tieneLibertades = false;
            bool grupo[TABLERO_MAX][TABLERO_MAX];
            for (int i = 0; i < partida.size; i++)
                for (int j = 0; j < partida.size; j++)
                    grupo[i][j] = false;

            DFS(posX, posY, celdaJugador, celdaOponente, visitado, &tieneLibertades, grupo);
            if (tieneLibertades) continue;

            for (int i = 0; i < partida.size; i++) {
                for (int j = 0; j < partida.size; j++) {
                    if (!grupo[i][j]) continue;
                    realizaronCapturas = true;
                    // marca la ficha como capturada
                    partida.tablero[i][j] = celdaOponente == CELDA_NEGRA ? CELDA_BLANCA_CAPT : CELDA_NEGRA_CAPT;
                    // actualiza el estado de 'ocupado'
                    ocupadas[i][j] = false;
                }
            }
        }
    }

    return realizaronCapturas;
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
        }
    }
}

bool ko(int x, int y, celda_t celdaJugador, celda_t celdaOponente) {
    if (partida.turno <= 2) return false;

    const int size = partida.size;
    const int puntajeJugador = partida.puntajeJugador;
    const int puntajeOponente = partida.puntajeOponente;
    char previo[size][size];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            previo[i][j] = partida.tablero[i][j];

    partida.tablero[x][y] = celdaJugador;
    const bool realizaronCapturas = capturas(celdaOponente, celdaJugador);
    bool esKo = realizaronCapturas;
    if (realizaronCapturas) {
        eliminarCapturadas();
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (partida.tablero[i][j] == partida.anterior[i][j]) continue;
                esKo = false;
                break;
            }
            if (!esKo) break;
        }
    }

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            partida.tablero[i][j] = previo[i][j];
    partida.puntajeJugador = puntajeJugador;
    partida.puntajeOponente = puntajeOponente;

    return esKo;
}

bool suicidio(int x, int y, celda_t celdaJugador, celda_t celdaOponente) {
    const int size = partida.size;
    char previo[size][size];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            previo[i][j] = partida.tablero[i][j];

    partida.tablero[x][y] = celdaJugador;
    const bool realizaCaptura = capturas(celdaOponente, celdaJugador);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            partida.tablero[i][j] = previo[i][j];

    if (realizaCaptura) return false;

    partida.tablero[x][y] = celdaJugador;
    const bool fueCapturado = capturas(celdaJugador, celdaOponente);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            partida.tablero[i][j] = previo[i][j];

    return fueCapturado;
}
