#include <locale.h>
#include <stdio.h>
#include <wchar.h>

#include "datos.h"
#include "menu.h"

void setup() {
    setupDatos();
    setlocale(LC_ALL, "");
}

int main() {
    setup();
    obtenerNombre();

    while (1) {
        printBienvenida();
        const int menu = obtenerMenu();
        if (menu == MENU_JUGAR) {
            const int orden = ejecutarMenuJugar();
            if (orden == VOLVER) continue;
            break;
        }

        if (menu == MENU_CONFIG) {
            const int orden = ejecutarMenuConfig();
            if (orden == VOLVER) continue;
            break;
        }
    }

    return 0;
}
