#include <stdbool.h>
#include <wchar.h>

#include "util.h"

#ifndef _MENU_DEFINIDO

#define _MENU_DEFINIDO

#define MENUS 4
enum MENU {
    MENU_JUGAR,
    MENU_REGLAS,
    MENU_CONFIG,
    MENU_STATS,
};

enum MENU_FLAG {
    REPETIR = -3,
    VOLVER,
    SALIR,
};

#define MENSAJE_ORDEN_LEN TITULO_LEN * 2

typedef struct MenuOrden {
    enum MENU_FLAG flag;
    wchar_t mensaje[MENSAJE_ORDEN_LEN];
} MenuOrden;

#define CONFIGS 3
enum CONFIG_MENU {
    CONFIG_NOMBRE,
    CONFIG_BORRAR_PARTIDAS,
    CONFIG_BORRAR_TODO,
};

#define STATS 6
#define STATS_POR_PAGE 25

void printBienvenida(bool repetirFlag);
int obtenerMenu();
const MenuOrden ejecutarMenuJugar();
const MenuOrden ejecutarMenuReglas();
const MenuOrden ejecutarMenuConfig();
const MenuOrden ejecutarMenuStats();

#endif
