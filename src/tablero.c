#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "datos.h"
#include "util.h"

GoPartida partida;

#define CELDA_J 'X'
#define CELDA_M 'O'

void puntajePorCantidad();

void crearTablero(int size) {
    partida.size = size;
    const int max = size;

    partida.tablero[1][1] = '+';
    partida.tablero[1][max] = '+';
    partida.tablero[max][max] = '+';
    partida.tablero[max][1] = '+';

    for (int i = 2; i < max; i++) {
        partida.tablero[max][i] = '+';
        partida.tablero[1][i] = '+';
        partida.tablero[i][1] = L'¦';
        partida.tablero[i][max] = L'¦';
        for (int j = 2; j < max; j++)
            partida.tablero[i][j] = L'¦';
    }
}

void printTablero() {
    const int size = partida.size;
    for (int i = 1; i < size + 1; i++) {
        for (int j = 1; j < size + 1; j++)
            printf(j + 1 < size + 1 ? "%c-" : "%c", partida.tablero[i][j]);
        printf("\n");
    }
    printf("\n");
}

void jugarTablero() {
    printTablero();
    const int size = partida.size;
    int turno = 0;
    partida.turnoNegras = true;
    bool ocupadas[size][size];
    for (int i = 1; i < size + 1; i++)
        for (int j = 1; j < size + 1; j++)
            ocupadas[i][j] = false;

    while (turno < 10) {  // condición de victoria
        int x, y;
        printf("\tTurno de %s\n\n", partida.turnoNegras ? "negras" : "blancas");
        printf("Insertar coordenada:\n");
        printf("x: ");
        scanf("%d", &x);
        printf("y: ");
        scanf("%d", &y);

        while (ocupadas[x][y] == true) {
            printf("Esa casilla ya esta ocupada!\n");
            printf("x: ");
            scanf("%d", &x);
            printf("y: ");
            scanf("%d", &y);
        }

        ocupadas[x][y] = true;
        partida.tablero[x][y] = partida.turnoNegras ? CELDA_J : CELDA_M;

        limpiarConsola();
        printTitulo();
        printTablero();

        partida.turnoNegras = !partida.turnoNegras;
        turno++;
    }

    partida.fecha = now();

    puntajePorCantidad();
    guardarPartida(partida);
}

// función que determina un puntaje inicial dependiendo de la cantidad de fichas en mesa de cada jugador
// falta incorporar puntaje por captura (a espera de parte de capturas)
// analizar si incorporar puntuación por area o modificar la regla japonesa en beneficio del programa
void puntajePorCantidad() {
    for (int i = 1; i < partida.size + 1; i++) {
        for (int j = 1; j < partida.size + 1; j++) {
            const wchar_t celda = partida.tablero[i][j];
            if (celda == CELDA_J) partida.puntajeJugador++;
            if (celda == CELDA_M) partida.puntajeMaquina++;
        }
    }
}
