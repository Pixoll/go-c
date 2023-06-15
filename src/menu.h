#ifndef _MENU_DEFINIDO

#define _MENU_DEFINIDO

#define MENUS 4
enum MENU {
    MENU_JUGAR,
    MENU_REGLAS,
    MENU_CONFIG,
    MENU_STATS,
};

#define CONFIGS 3
enum MENU_CONFIG {
    CONFIG_NOMBRE,
    CONFIG_BORRAR_PARTIDAS,
    CONFIG_BORRAR_TODO,
};

#define VOLVER -1
#define SALIR -2

void printBienvenida();
void obtenerNombre();
int obtenerMenu();
int ejecutarMenuJugar();
int ejecutarMenuConfig();

#endif
