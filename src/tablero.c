#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "datos.h"
#include "util.h"

GoPartida partida;

void porCantidad(); //void de funcion

void crearTablero(int size)
{
    partida.size = size;
    const int max = size;

    partida.tablero[1][1] = '+';
    partida.tablero[1][max] = '+';
    partida.tablero[max][max] = '+';
    partida.tablero[max][1] = '+';

    for (int i = 2; i < max; i++)
    {
        partida.tablero[max][i] = '+';
        partida.tablero[1][i] = '+';
        partida.tablero[i][1] = L'¦';
        partida.tablero[i][max] = L'¦';
        for (int j = 2; j < max; j++)
            partida.tablero[i][j] = L'¦';
    }
}

void printTablero()
{
    const int size = partida.size;
    for (int i = 1; i < size+1; i++)
    {
        for (int j = 1; j < size+1; j++)
            printf(j+1 < size +1 ? "%c-" : "%c", partida.tablero[i][j]);
        printf("\n");
    }
    printf("\n");
}

bool turnoNegras = true;

void jugarTablero()
{
    printTablero();
    int x, y;
    const int size = partida.size;
    int turno = 0;
    bool ocupadas[size][size];
    for (int i = 1; i < size+1; i++)
        for (int j = 1; j < size+1; j++)
            ocupadas[i][j] = false;
    while (turno < 100)
    { // condición de victoria
        printf("\tTurno de %s\n\n", turnoNegras ? "negras" : "blancas");
        printf("Insertar coordenada:\n");
        printf("x: ");
        scanf("%d", &x);
        printf("y: ");
        scanf("%d", &y);

        while (ocupadas[x][y] == true)
        {
            printf("Esa casilla ya esta ocupada!\n");
            printf("x: ");
            scanf("%d", &x);
            printf("y: ");
            scanf("%d", &y);
        }
        ocupadas[x][y] = true;
        partida.tablero[x][y] = turnoNegras ? 'X' : 'O';
        limpiarConsola();
        printTitulo();
        printTablero();
        turnoNegras = !turnoNegras;
        turno++;
    }
    
    guardarPartida(partida);
}
// funcion que determina un puntaje inicial dependiendo de la cantidad de fichas en mesa de cada jugador
//falta incroporar puntaje por captura (a espera de parte de capturas)
//analizar si incorporar puntuacion por area o modificar la regla japonesa en beneficio del programa
void porCantidad(){
    int pj=0;
    int pm=0;
    for(int i=1; i<partida.size+1; i++){
        for(int j=1; j<partida.size+1; j++){
            if (partida.tablero[i][j]==88){
                pj++;
            }
            else if (partida.tablero[i][j]==79){
                pm++;
            }
        }
    }
    partida.puntajeJugador=pj;
    partida.puntajeMaquina=pm;
}
