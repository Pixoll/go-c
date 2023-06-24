#include <locale.h>
#include <stdbool.h>
#include <stdio.h>

#include "datos.h"
#include "menu.h"

void setup() {
    setupDatos();
    setlocale(LC_ALL, "");
    fwide(stdout, 1);
}

int main() {
    setup();

    int menu = -1;
    bool repetir = false;
    while (true) {
        printBienvenida(repetir);
        repetir = false;
        if (menu == -1) menu = obtenerMenu();
        const int orden = menu == MENU_JUGAR    ? ejecutarMenuJugar()
                          : menu == MENU_REGLAS ? ejecutarMenuReglas()
                          : menu == MENU_CONFIG ? ejecutarMenuConfig()
                          : menu == MENU_STATS  ? ejecutarMenuStats()
                                                : menu;  // solo puede ser SALIR

        if (orden == SALIR)
            break;
        else if (orden == VOLVER)
            menu = -1;
        else if (orden == REPETIR)
            repetir = true;
    }

    return 0;
}
