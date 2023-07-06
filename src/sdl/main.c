#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "../common/datos.h"
#include "globals.h"
#include "util.h"

void initSDL();
void closeSDL();
void dibujarMenuPrincipal();

int main(int argc, char *argv[]) {
    setupDatos();
    initSDL();
    dibujarMenuPrincipal();

    SDL_Event evento;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&evento) != 0) {
            if (evento.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    closeSDL();
    return 0;
}

void initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL no se pudo iniciar. SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    ventana = SDL_CreateWindow("Go", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 640,
                               SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
    if (ventana == NULL) {
        printf("Ventana no pudo ser creada. SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_SetWindowResizable(ventana, false);

    SDL_Event evento;
    SDL_PollEvent(&evento);

    SDL_Surface *superficie = SDL_GetWindowSurface(ventana);
    Uint32 color = SDL_MapRGB(superficie->format, 0xFF, 0xFF, 0xAA);
    SDL_FillRect(superficie, NULL, color);
    SDL_UpdateWindowSurface(ventana);

    obtenerVentanaDimension();
}

void closeSDL() {
    SDL_DestroyWindow(ventana);
    SDL_Quit();
}

void dibujarMenuPrincipal() {
    const SDL_Rect banner = crearRect(50, 50, ventanaW - 100, 150);
    SDL_Surface *superficie = SDL_GetWindowSurface(ventana);
    SDL_FillRect(superficie, &banner, mapRBG(superficie, "eca742"));
    SDL_UpdateWindowSurface(ventana);
}
