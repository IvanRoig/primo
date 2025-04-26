#include "sdl2.h"

void inicializarTablero(int cf, SDL_Rect fr, SDL_Renderer* render) {
    int cellW = fr.w / cf;
    int cellH = fr.h / cf;

    SDL_SetRenderDrawColor(render, 255, 255, 255, 255); // Blanco
    SDL_RenderClear(render);

    SDL_SetRenderDrawColor(render, 0, 0, 0, 255); // Negro

    for (int i = 1; i < cf; i++) {
        SDL_RenderDrawLine(render, fr.x + i * cellW, fr.y, fr.x + i * cellW, fr.y + fr.h);
        SDL_RenderDrawLine(render, fr.x, fr.y + i * cellH, fr.x + fr.w, fr.y + i * cellH);
    }

    SDL_RenderPresent(render);
}

void renderizarTablero(int** mat, int cf, SDL_Rect fr, SDL_Renderer* render) {
    inicializarTablero(cf, fr, render); // Redibuja líneas
    int cellW = fr.w / cf;
    int cellH = fr.h / cf;

    for (int fila = 0; fila < cf; fila++) {
        for (int col = 0; col < cf; col++) {
            if (mat[fila][col] != 0) {
                SDL_Rect celda = {
                    fr.x + col * cellW,
                    fr.y + fila * cellH,
                    cellW,
                    cellH
                };
                if (mat[fila][col] == 1)
                    dibujarX(render, celda);
                else
                    dibujarO(render, celda);
            }
        }
    }

    SDL_RenderPresent(render);
}

void dibujarX(SDL_Renderer* render, SDL_Rect celda) {
    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
    SDL_RenderDrawLine(render, celda.x, celda.y, celda.x + celda.w, celda.y + celda.h);
    SDL_RenderDrawLine(render, celda.x, celda.y + celda.h, celda.x + celda.w, celda.y);
}

void dibujarO(SDL_Renderer* render, SDL_Rect celda) {
    SDL_SetRenderDrawColor(render, 0, 0, 255, 255);
    int radio = celda.w / 4;
    int cx = celda.x + celda.w / 2;
    int cy = celda.y + celda.h / 2;

    for (int w = 0; w < radio * 2; w++) {
        for (int h = 0; h < radio * 2; h++) {
            int dx = radio - w;
            int dy = radio - h;
            if ((dx * dx + dy * dy) >= (radio - 1) * (radio - 1) && (dx * dx + dy * dy) <= (radio * radio)) {
                SDL_RenderDrawPoint(render, cx + dx, cy + dy);
            }
        }
    }
}
