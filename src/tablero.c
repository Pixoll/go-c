#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "datos.h"
#include "util.h"

GoPartida partida;

void crearTablero(int size) {
    partida.size = size;
    const int max = size - 1;

    partida.tablero[0][0] = '+';
    partida.tablero[0][max] = '+';
    partida.tablero[max][max] = '+';
    partida.tablero[max][0] = '+';

    for (int i = 1; i < max; i++) {
        partida.tablero[max][i] = '+';
        partida.tablero[0][i] = '+';
        partida.tablero[i][0] = L'¦';
        partida.tablero[i][max] = L'¦';
        for (int j = 1; j < max; j++)
            partida.tablero[i][j] = L'¦';
    }
}

void printTablero() {
    const int size = partida.size;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
            printf(j < size - 1 ? "%c-" : "%c", partida.tablero[i][j]);
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
        x--;
        y--;

        while (ocupadas[x][y] == true) {
            printf("Esa casilla ya esta ocupada!\n");
            printf("x: ");
            scanf("%d", &x);
            printf("y: ");
            scanf("%d", &y);
            x--;
            y--;
        }
        ocupadas[x][y] = true;
        partida.tablero[x][y] = turnoNegras ? 'O' : '@';

        limpiarConsola();
        printTitulo();
        printTablero();

        turnoNegras = !turnoNegras;
        turno++;
    }

    guardarPartida(partida);
}
