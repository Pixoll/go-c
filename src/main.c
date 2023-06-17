#include <locale.h>
#include <stdbool.h>
#include <stdio.h>

#include "datos.h"
#include "menu.h"

void setup() {
    setupDatos();
    setlocale(LC_ALL, "");
}

int main() {
    setup();

    int menu = -1;
    bool repetir = false;
    while (1) {
        printBienvenida(repetir);
        repetir = false;
        if (menu == -1) menu = obtenerMenu();

        if (menu == MENU_JUGAR) {
            const int orden = ejecutarMenuJugar();
            if (orden == VOLVER) {
                menu = -1;
                continue;
            }
            // break;
        }

        if (menu == MENU_CONFIG) {
            const int orden = ejecutarMenuConfig();
            if (orden == REPETIR) {
                repetir = true;
                continue;
            }
            if (orden == VOLVER) {
                menu = -1;
                continue;
            }
            break;
        }

        if (menu == MENU_STATS) {
            const int orden = ejecutarMenuStats();
            if (orden == VOLVER) {
                menu = -1;
                continue;
            }
            break;
        }
    }

    return 0;
}
