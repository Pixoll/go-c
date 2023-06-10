#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int tablero() {
    int tablero;
    printf("1.9x9\t2.13x13\t3.19x19\n");
    scanf("%d", &tablero);

    const int n = tablero == 1 ? 9 : tablero == 2 ? 13
                                                  : 19;

    int a[n][n];
    a[0][0] = '+';
    a[0][n - 1] = '+';
    a[n - 1][n - 1] = '+';
    a[n - 1][0] = '+';

    for (int j = 1; j < n - 1; j++)
        a[0][j] = '+';

    for (int j = 1; j < n - 1; j++)
        a[n - 1][j] = '+';

    for (int i = 1; i < n - 1; i++) {
        a[i][0] = L'¦';
        for (int j = 1; j < n - 1; j++)
            a[i][j] = L'¦';
        a[i][n - 1] = L'¦';
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j < n - 1)
                printf("%c-", a[i][j]);
            else
                printf("%c", a[i][j]);
        }
        printf("\n");
    }

    printf("\n\t");

    int cx, cy;
    bool turnoNegras = true;
    int b[n][n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            b[i][j] = 0;

    while (turnoNegras < 100) {  // condición de victoria
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

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (j < n - 1)
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
