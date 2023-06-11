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

    for (int i = 1; i < size - 1; i++) {
        partida.tablero[size - 1][i] = '+';
        partida.tablero[0][i] = '+';
        partida.tablero[i][0] = L'¦';
        partida.tablero[i][size - 1] = L'¦';
        for (int j = 1; j < size - 1; j++)
            partida.tablero[i][j] = L'¦';
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
    bool ocupadas[size][size];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            ocupadas[i][j] = false;

    while (turno < 100) {  // condición de victoria
        int x, y;
        printf("\tTurno de %s\n\n", turnoNegras ? "negras" : "blancas");
        printf("Insertar coordenada:\n");
        printf("x: ");
        scanf("%d", &x);
        printf("y: ");
        scanf("%d", &y);

        while (ocupadas[y - 1][x - 1] == true) {
            printf("Esa casilla ya esta ocupada!\n");
            printf("x: ");
            scanf("%d", &x);
            printf("y: ");
            scanf("%d", &y);
        }
        ocupadas[y - 1][x - 1] = true;
        partida.tablero[y - 1][x - 1] = turnoNegras ? 'O' : '@';

        limpiarConsola();
        printTitulo();
        printTablero();

        turnoNegras = !turnoNegras;
        turno++;
    }

    guardarPartida(partida);
}
