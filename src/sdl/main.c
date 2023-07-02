#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SDL_Window* window = NULL;          // ventana principal
SDL_Surface* screenSurface = NULL;  // superficie principal.

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 1280;
const int numBox = 9;
const int widthBox = (float)SCREEN_WIDTH / (float)numBox;
const int heightBox = (float)SCREEN_HEIGHT / (float)numBox;

/*
const int numBox = 6;
const int widthBox = 100;
const int heightBox = 100;

const int SCREEN_HEIGHT = heightBox * numBox + 10;
const int SCREEN_WIDTH = widthBox * numBox + 10;
*/

bool initSDL();

void closeSDL();

int checkCar();

void checkBox(int xMouse, int yMouse, int* xIndex, int* yIndex);

int main() {
    if (!initSDL()) {
        printf("Fallo al iniciar\n");
        return 0;
    }
    bool quit = false;

    SDL_Event e;
    SDL_Rect board[numBox][numBox];
    SDL_Rect boardBackground[numBox][numBox];

    for (int i = 0; i < numBox; i++) {
        for (int j = 0; j < numBox; j++) {
            board[i][j].x = (i * widthBox) + 10;
            board[i][j].y = (j * heightBox) + 10;
            board[i][j].w = widthBox - 10;
            board[i][j].h = heightBox - 10;
            boardBackground[i][j].x = (i * widthBox) + 10;
            boardBackground[i][j].y = (j * heightBox) + 10;
            boardBackground[i][j].w = widthBox - 10;
            boardBackground[i][j].h = heightBox - 10;
            // printf("%d %d %d %d\n", board[i][j].x, board[i][j].y, board[i][j].w, board[i][j].h);
        }
    }

    while (!quit) {
        int xIndex = -1;
        int yIndex = -1;
        while (SDL_PollEvent(&e) != 0) {
            // User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_MOUSEMOTION) {
                // printf("Mouse Deslizado\n");
            }
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                printf("Mouse click pulsado\n");
                int x, y;
                SDL_GetMouseState(&x, &y);
                checkBox(x, y, &xIndex, &yIndex);
                printf("%d %d\n", xIndex, yIndex);
            } else if (e.type == SDL_MOUSEBUTTONUP) {
                printf("Mouse click soltado\n");
            }
        }
        screenSurface = SDL_GetWindowSurface(window);                                            // Obtiene la superficie de la ventana
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xAA));  // Rellena la superficie con un cuadrado blanco.
        SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xAA));  // Rellena la superficie con un cuadrado blanco.
        for (int i = 0; i < numBox; i++) {
            for (int j = 0; j < numBox; j++) {
                if (i == xIndex && j == yIndex) {
                    printf("hola\n");
                    board[i][j].x = -100;
                    board[i][j].y = -100;
                }
                SDL_FillRect(screenSurface, &boardBackground[i][j], SDL_MapRGB(screenSurface->format, 0xAA, 0xFF, 0xFF));  // lo agrega a la superfice y lo colorea.
                SDL_FillRect(screenSurface, &board[i][j], SDL_MapRGB(screenSurface->format, 0xAB, 0xAB, 0xAC));            // lo agrega a la superfice y lo colorea.
            }
        }
        SDL_UpdateWindowSurface(window);  // Actualiza el contenido de la ventana.
    }
    closeSDL();

    return 0;
}

bool initSDL() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        // printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        printf("SDL error init video\n");
        return false;
    } else {
        // Create window
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            return false;
        } else {
            // screenSurface = SDL_GetWindowSurface( window );  //Obtiene la superficie de la ventana
            printf("ok");
        }
    }
    return true;
}

void closeSDL() {
    // Destroy window
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();
}

void checkBox(int xMouse, int yMouse, int* xIndex, int* yIndex) {
    *xIndex = -1;
    *yIndex = -1;
    for (int i = 0; i < numBox; i++) {
        for (int j = 0; j < numBox; j++) {
            int leftLimitX = i * widthBox;
            int rightLimitX = (i + 1) * widthBox;
            int upLimitY = j * heightBox;
            int downLimitY = (j + 1) * heightBox;

            int count = 0;
            if (xMouse > leftLimitX && xMouse < rightLimitX) {
                count++;
            }
            if (yMouse > upLimitY && yMouse < downLimitY) {
                count++;
            }
            if (count == 2) {
                *xIndex = i;
                *yIndex = j;
                return;
            }
        }
    }
    return;
}
