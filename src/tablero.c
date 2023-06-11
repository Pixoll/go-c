#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "datos.h"
#include "util.h"

GoPartida partida;

void crearTablero(int size) {
    partida.size = size;

    partida.tablero[0][0] = '+';
    partida.tablero[0][size - 1] = '+';
    partida.tablero[size - 1][size - 1] = '+';
    partida.tablero[size - 1][0] = '+';

    for (int j = 1; j < size - 1; j++)
        partida.tablero[0][j] = '+';

    for (int j = 1; j < size - 1; j++)
        partida.tablero[size - 1][j] = '+';

    for (int i = 1; i < size - 1; i++) {
        partida.tablero[i][0] = L'¦';
        for (int j = 1; j < size - 1; j++)
            partida.tablero[i][j] = L'¦';
        partida.tablero[i][size - 1] = L'¦';
    }
}

void printTablero() {
    for (int i = 0; i < partida.size; i++) {
        for (int j = 0; j < partida.size; j++)
            printf(j < partida.size - 1 ? "%c-" : "%c", partida.tablero[i][j]);
        printf("\n");
    }
    printf("\n");
}

void jugarTablero() {
    printTablero();

    const int size = partida.size;
    int turno = 0;
    bool turnoNegras = true;
    short ocupadas[size][size];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            ocupadas[i][j] = 0;

    while (turno < 100) {  // condición de victoria
        int x, y;
        printf("\tTurno de %s\n\n", turnoNegras ? "negras" : "blancas");
        printf("Insertar coordenada:\n");
        printf("x: ");
        scanf("%d", &x);
        printf("y: ");
        scanf("%d", &y);

        ocupadas[y - 1][x - 1] = 1;
        while (1 < ocupadas[y - 1][x - 1]) {
            printf("Esa casilla ya esta ocupada!\n");
            printf("x: ");
            scanf("%d", &x);
            printf("y: ");
            scanf("%d", &y);
            ocupadas[y - 1][x - 1] = 1;
        }
        partida.tablero[y - 1][x - 1] = turnoNegras ? 'O' : '@';

        limpiarConsola();
        printTitulo();
        printTablero();

        turnoNegras = !turnoNegras;
        turno++;
    }

    guardarPartida(partida);
}
