#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

int tablero(int size) {
    int a[size][size];
    a[0][0] = '+';
    a[0][size - 1] = '+';
    a[size - 1][size - 1] = '+';
    a[size - 1][0] = '+';

    for (int j = 1; j < size - 1; j++)
        a[0][j] = '+';

    for (int j = 1; j < size - 1; j++)
        a[size - 1][j] = '+';

    for (int i = 1; i < size - 1; i++) {
        a[i][0] = L'¦';
        for (int j = 1; j < size - 1; j++)
            a[i][j] = L'¦';
        a[i][size - 1] = L'¦';
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (j < size - 1)
                printf("%c-", a[i][j]);
            else
                printf("%c", a[i][j]);
        }
        printf("\n");
    }

    printf("\n\t");

    int cx, cy;
    bool turnoNegras = true;
    int b[size][size];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            b[i][j] = 0;

    while (turnoNegras < 100) {  // condición de victoria
        printf("Turno de %s\n", turnoNegras ? "negras" : "blancas");
        printf("\ninsertar coordenada:\n");
        printf("x:\t");
        scanf("%d", &cx);
        printf("y:\t");
        scanf("%d", &cy);

        b[cy - 1][cx - 1] += 1;
        while (1 < b[cy - 1][cx - 1]) {
            printf("esa casilla ya esta ocupada!\n");
            printf("x:\t");
            scanf("%d", &cx);
            printf("y:\t");
            scanf("%d", &cy);
            b[cy - 1][cx - 1] += 1;
        }
        a[cy - 1][cx - 1] = turnoNegras ? 'O' : '@';

        limpiarConsola();
        printTitulo();

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (j < size - 1)
                    printf("%c-", a[i][j]);
                else
                    printf("%c", a[i][j]);
            }
            printf("\n");
        }
        printf("\n\t");

        turnoNegras = !turnoNegras;
    }

    return 0;
}
