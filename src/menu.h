#include <stdbool.h>

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
enum CONFIG_MENU {
    CONFIG_NOMBRE,
    CONFIG_BORRAR_PARTIDAS,
    CONFIG_BORRAR_TODO,
};

#define STATS 6
#define STATS_POR_PAGE 25

enum MENU_FLAG {
    REPETIR = -3,
    VOLVER,
    SALIR,
};

void printBienvenida(bool repetirFlag);
int obtenerMenu();
int ejecutarMenuJugar();
int ejecutarMenuReglas();
int ejecutarMenuConfig();
int ejecutarMenuStats();

#endif
