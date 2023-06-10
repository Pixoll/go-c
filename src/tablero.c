#include <stdio.h>
#include <stdlib.h>

int tablero() {
    int n, e, tablero;

    printf("1.9x9\t2.13x13\t3.19x19\n");
    scanf("%d", &tablero);

    if (tablero == 1) {
        n = 9;
    }
    if (tablero == 2) {
        n = 13;
    }
    if (tablero == 3) {
        n = 19;
    }
    int lista[n * n];
    int a[n][n];
    e = '-';
    a[0][0] = '+';
    a[0][n - 1] = '+';
    a[n - 1][n - 1] = '+';
    a[n - 1][0] = '+';
    for (int j = 1; j < n - 1; j++) {
        a[0][j] = '+';
    }
    for (int j = 1; j < n - 1; j++) {
        a[n - 1][j] = '+';
    }
    for (int i = 1; i < n - 1; i++) {
        a[i][0] = L'¦';
        for (int j = 1; j < n - 1; j++) {
            a[i][j] = L'¦';
        }
        a[i][n - 1] = L'¦';
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j < n - 1) {
                printf("%c", a[i][j]);
                printf("%c", e);
            } else {
                printf("%c", a[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n\t");

    int cx, cy;
    int c = 0;
    int b[n][n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            b[i][j] = 0;
        }
    }
    while (c < 100) {  // condicion de victoria
        printf("\nincertar cordenada:\n");
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
        if (c % 2 == 0) {
            a[cy - 1][cx - 1] = 'O';
        } else {
            a[cy - 1][cx - 1] = '@';
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (j < n - 1) {
                    printf("%c", a[i][j]);
                    printf("%c", e);
                } else {
                    printf("%c", a[i][j]);
                }
            }

            printf("\n");
        }
        printf("\n\t");

        c += 1;
    }
    return 0;
}
