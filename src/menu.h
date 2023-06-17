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

#define VOLVER -1
#define SALIR -2

void printBienvenida();
int obtenerMenu();
int ejecutarMenuJugar();
int ejecutarMenuConfig();
int ejecutarMenuStats();

#endif
