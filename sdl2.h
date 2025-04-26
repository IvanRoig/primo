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

#include <SDL.h>

#define TAMMAX 100


void inicializarTablero(int cf, SDL_Rect fr, SDL_Renderer* render);
void renderizarTablero(int** mat, int cf, SDL_Rect fr, SDL_Renderer* render);
void dibujarX(SDL_Renderer* render, SDL_Rect celda);
void dibujarO(SDL_Renderer* render, SDL_Rect celda);


#endif // MATRICES_H_INCLUDED
