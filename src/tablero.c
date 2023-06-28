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
    CELDA_EMPTY,
    CELDA_NEGRA_CAPT,
    CELDA_BLANCA_CAPT,
    CELDA_ANALIZADA,
};
typedef enum CELDA celda_t;

const wchar_t celdas[CELDAS] = {'?', 'X', 'O', '+', L'¦', '?', '?', '?', '?'};
const int dx[4] = {-1, 1, 0, 0};
const int dy[4] = {0, 0, -1, 1};

GoPartida partida;
bool ocupadas[TABLERO_MAX][TABLERO_MAX];

bool esMaquina() {
    return strlen(partida.oponente) == 0;
}

void puntajePorCantidad();
void puntajePorArea();
void capturas(celda_t celdaJugador, celda_t celdaOponente);
void eliminarCapturadas();
void jugarMaquina(int *px, int *py);

void crearTablero(int size, char *oponente) {
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

void jugarTablero() {
    printTablero();
    const int size = partida.size;
    int turno = 0;
    const wchar_t *oponente = esMaquina() ? L"máquina" : strtowcs(partida.oponente);

    while (turno < 100) {  // condición de victoria
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
            wprintf(L"Insertar coordenada:\n");
            wprintf(L"Fila: ");
            scanf("%d", &x);
            wprintf(L"Columna: ");
            scanf("%d", &y);
            if (x == 0 && y == 0) {
                break;
            }

            while (ocupadas[x][y] == true) {
                wprintf(L"Esa casilla ya esta ocupada!\n");
                wprintf(L"Fila: ");
                scanf("%d", &x);
                wprintf(L"Columna: ");
                scanf("%d", &y);
                if (x == 0 && y == 0) {
                    break;
                }
            }
        }

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

    partida.terminada = true;
    partida.fecha = now();

    puntajePorCantidad();
    puntajePorArea();

    wprintf(L"Puntaje %s: %d\n", config.nombre, partida.puntajeJugador);
    wprintf(L"Puntaje %ls: %d\n\n", oponente, partida.puntajeOponente);

    int end = 0;
    while (end != 1) {
        wprintf(L"Ingrese dos 1 para volver al inicio: ");
        scanf("%d \n", &end);
    }

    guardarPartida(partida);
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
                    const int x = i + dx[i];
                    const int y = j + dy[i];
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

    // cambia el area de CELDA_EMPTY por CELDA_ANALIZADA para que esta no se vuelva a analizar nuevamente
    for (int i = 1; i <= partida.size; i++) {
        for (int j = 1; j <= partida.size; j++) {
            if (partida.tablero[i][j] != CELDA_EMPTY) continue;
            partida.tablero[i][j] = CELDA_ANALIZADA;
        }
    }
}

// analizamos el area de CELDA_EMPTY verificando si solo la rodea un tipo de ficha o no
void verificarArea(int total) {
    int negras = 0;
    int blancas = 0;
    int otro = 0;
    // ciclo que contabiliza los tipos de zonas que rodean el area de CELDA_EMPTY
    for (int i = 1; i <= partida.size; i++) {
        for (int j = 1; j <= partida.size; j++) {
            if (partida.tablero[i][j] != CELDA_EMPTY) continue;
            if (partida.tablero[i + 1][j] == CELDA_NEGRA || partida.tablero[i - 1][j] == CELDA_NEGRA || partida.tablero[i][j + 1] == CELDA_NEGRA || partida.tablero[i][j - 1] == CELDA_NEGRA) {
                negras++;
            }
            if (partida.tablero[i + 1][j] == CELDA_BLANCA || partida.tablero[i - 1][j] == CELDA_BLANCA || partida.tablero[i][j + 1] == CELDA_BLANCA || partida.tablero[i][j - 1] == CELDA_BLANCA) {
                blancas++;
            }
            if (partida.tablero[i + 1][j] != CELDA_BLANCA && partida.tablero[i - 1][j] != CELDA_BLANCA && partida.tablero[i][j + 1] != CELDA_BLANCA && partida.tablero[i][j - 1] != CELDA_BLANCA && partida.tablero[i + 1][j] != CELDA_NEGRA && partida.tablero[i - 1][j] != CELDA_NEGRA && partida.tablero[i][j + 1] != CELDA_NEGRA && partida.tablero[i + j][j - 1] != CELDA_NEGRA) {
                otro++;
            }
        }
    }

    // si el area es rodeada solo por fichas negras o otro le asigna puntaje a jugador blanco
    if (negras != 0 && blancas == 0 && otro < negras * 2) {
        partida.puntajeJugador += total;
        // lo mismo con fichas blancas y con puntos al jugador blanco
    } else if (blancas != 0 && negras == 0 && otro < blancas * 2) {
        partida.puntajeOponente += total;
    }

    // de no cumplirse ninguna condición anterior se asume que el area es neutra y no se le otorgara puntaje a ningún jugador
    // mas a delante se pueden implementar mas condiciones de area neutra haciendo las condiciones del juego mas realista
}

// si quieren probar la contabilización de puntajes en una condición un tanto extrema seleccione jugar y el tablero de 9x9 (1) y coloque
// 1 2 3 1 2 2 4 2 2 3 5 2 2 4 6 2 3 5 7 3 4 4 7 4 4 3 7 5 5 3 7 6 6 3 8 6 6 4 9 6 6 5 4 8 6 6 5 7 5 6 5 9 4 7 6 7 3 7 6 9 2 7 7 8 1 7

// cva

// busca y recorre los grupos de fichas, los marca como que tienen o no libertades
void DFS(int posX, int posY, celda_t celdaJugador, celda_t celdaOponente, bool visitado[TABLERO_MAX][TABLERO_MAX], bool *tieneLibertades, int grupo[TABLERO_MAX][TABLERO_MAX]) {
    visitado[posX][posY] = true;
    grupo[posX][posY] = 1;

    for (int i = 0; i < 4; i++) {
        int nx = posX + dx[i];
        int ny = posY + dy[i];

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

// saca las fichas capturadas
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
