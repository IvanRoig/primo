#ifndef MATRICES_H_INCLUDED
#define MATRICES_H_INCLUDED
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef __MINGW32__
#define SDL_MAIN_HANDLED
#include <SDL_main.h>
#endif
#include <stdbool.h>
#include <SDL.h>


#define TAMMAX 100
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 400

typedef struct {
    SDL_Rect rect;
    const char* label;
} Button;


void inicializarTablero(int cf, SDL_Rect fr, SDL_Renderer* render);
void renderizarTablero(int** mat, int cf, SDL_Rect fr, SDL_Renderer* render);
void dibujarX(SDL_Renderer* render, SDL_Rect celda);
void dibujarO(SDL_Renderer* render, SDL_Rect celda);
bool isMouseOver(SDL_Rect rect, int x, int y);
void manejarClick(SDL_Event e, int** mat, int cf, SDL_Rect tablero, int* turno);



#endif // MATRICES_H_INCLUDED
