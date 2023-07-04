#include <SDL2/SDL.h>

SDL_Window* ventana = NULL;
int ventanaH = -1;
int ventanaW = -1;

void obtenerVentanaDimension() {
    if (ventanaH != -1 || ventanaW != -1) return;
    SDL_Surface *superficie = SDL_GetWindowSurface(ventana);
    ventanaH = superficie->h;
    ventanaW = superficie->w;
}
