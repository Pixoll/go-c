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

    int cx, cy, turno = 0, size = partida.size;
    bool turnoNegras = true;
    int b[size][size];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            b[i][j] = 0;

    while (turno < 100) {  // condición de victoria
        printf("\tTurno de %s\n\n", turnoNegras ? "negras" : "blancas");
        printf("Insertar coordenada:\n");
        printf("x: ");
        scanf("%d", &cx);
        printf("y: ");
        scanf("%d", &cy);

        b[cy - 1][cx - 1] += 1;
        while (1 < b[cy - 1][cx - 1]) {
            printf("Esa casilla ya esta ocupada!\n");
            printf("x: ");
            scanf("%d", &cx);
            printf("y: ");
            scanf("%d", &cy);
            b[cy - 1][cx - 1] += 1;
        }
        partida.tablero[cy - 1][cx - 1] = turnoNegras ? 'O' : '@';

        limpiarConsola();
        printTitulo();
        printTablero();

        turnoNegras = !turnoNegras;
        turno++;
    }

}
