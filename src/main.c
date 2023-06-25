#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <wchar.h>

#include "datos.h"
#include "menu.h"
#include "util.h"

void setup() {
    setupDatos();
    setlocale(LC_ALL, "");
    fwide(stdout, 1);
}

int main() {
    setup();

    int menu = -1;
    wchar_t mensaje[MENSAJE_ORDEN_LEN] = L"";
    bool repetir = false;
    while (true) {
        printBienvenida(repetir);
        if (wcslen(mensaje) != 0) {
            wprintConLineLimit(mensaje, TITULO_LEN);
            wprintf(L"\n\n");
        }

        repetir = false;
        if (menu == -1) menu = obtenerMenu();

        const MenuOrden orden = menu == MENU_JUGAR    ? ejecutarMenuJugar()
                                : menu == MENU_REGLAS ? ejecutarMenuReglas()
                                : menu == MENU_CONFIG ? ejecutarMenuConfig()
                                : menu == MENU_STATS  ? ejecutarMenuStats()
                                                      : (MenuOrden){menu, L""};  // solo puede ser SALIR

        wcsncpy(mensaje, orden.mensaje, MENSAJE_ORDEN_LEN);

        if (orden.flag == SALIR)
            break;
        else if (orden.flag == VOLVER)
            menu = -1;
        else if (orden.flag == REPETIR)
            repetir = true;
    }

    return 0;
}
