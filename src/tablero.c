#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "datos.h"
#include "util.h"

t_char **tablero;
int tableroSize;

void crearTablero(int size) {
    tableroSize = size;
    // declarar espacio usado
    tablero = malloc(size * sizeof(t_char *));
    for (int i = 0; i < size; i++)
        tablero[i] = malloc(size * sizeof(t_char));

    tablero[0][0] = '+';
    tablero[0][size - 1] = '+';
    tablero[size - 1][size - 1] = '+';
    tablero[size - 1][0] = '+';

    for (int j = 1; j < size - 1; j++)
        tablero[0][j] = '+';

    for (int j = 1; j < size - 1; j++)
        tablero[size - 1][j] = '+';

    for (int i = 1; i < size - 1; i++) {
        tablero[i][0] = L'¦';
        for (int j = 1; j < size - 1; j++)
            tablero[i][j] = L'¦';
        tablero[i][size - 1] = L'¦';
    }
}

void printTablero() {
    for (int i = 0; i < tableroSize; i++) {
        for (int j = 0; j < tableroSize; j++)
            printf(j < tableroSize - 1 ? "%c-" : "%c", tablero[i][j]);
        printf("\n");
    }
    printf("\n");
}

void destruirTablero() {
    for (int i = 0; i < tableroSize; i++)
        free(tablero[i]);
    free(tablero);
}

void jugarTablero() {
    printTablero();

    int cx, cy;
    bool turnoNegras = true;
    int b[tableroSize][tableroSize];
    for (int i = 0; i < tableroSize; i++)
        for (int j = 0; j < tableroSize; j++)
            b[i][j] = 0;

    while (turnoNegras < 100) {  // condición de victoria
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
        tablero[cy - 1][cx - 1] = turnoNegras ? 'O' : '@';

        limpiarConsola();
        printTitulo();
        printTablero();

        turnoNegras = !turnoNegras;
    }

    destruirTablero();
}
