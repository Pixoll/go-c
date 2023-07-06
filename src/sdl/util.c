#include "../common/util.h"

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>

SDL_Rect crearRect(int x, int y, int w, int h) {
    return (SDL_Rect){x, y, w, h};
}

// hexColor debe ser de formato "abc123" sin el # con un máximo de 6 caracteres (7 con nulo)
Uint32 mapRBG(SDL_Surface *superficie, char hexColor[7]) {
    const int r = strtol(substr(hexColor, 0, 2), NULL, 16),
              g = strtol(substr(hexColor, 2, 4), NULL, 16),
              b = strtol(substr(hexColor, 4, 6), NULL, 16);
    return SDL_MapRGB(superficie->format, r, g, b);
}
