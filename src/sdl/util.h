#include <SDL2/SDL.h>

#ifndef _SDL_UTIL_DEFINIDO

#define _SDL_UTIL_DEFINIDO

SDL_Rect crearRect(int x, int y, int w, int h);
Uint32 mapRBG(SDL_Surface *superficie, char hexColor[7]);

#endif
