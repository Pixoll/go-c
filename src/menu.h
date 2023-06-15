#ifndef _MENU_DEFINIDO

#define _MENU_DEFINIDO

#define MENU_JUGAR 0
#define MENU_REGLAS 1
#define MENU_CONFIG 2
#define MENU_STATS 3
#define MENUS 4

#define CONFIG_NOMBRE 0
#define CONFIG_BORRAR_PARTIDAS 1
#define CONFIG_BORRAR_TODO 2
#define CONFIGS 3

#define VOLVER -1
#define SALIR -2

void printBienvenida();
void obtenerNombre();
int obtenerMenu();
int ejecutarMenuJugar();
int ejecutarMenuConfig();

#endif
