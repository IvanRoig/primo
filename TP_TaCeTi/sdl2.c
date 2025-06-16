#include "sdl2.h"

void inicializarTablero(int cf, SDL_Rect fr, SDL_Renderer* render)
{
    int cellW = fr.w / cf;
    int cellH = fr.h / cf;


    SDL_SetRenderDrawColor(render, 0, 0, 0, 255); // Negro

    for (int i = 1; i < cf; i++)
    {
        SDL_RenderDrawLine(render, fr.x + i * cellW, fr.y, fr.x + i * cellW, fr.y + fr.h);
        SDL_RenderDrawLine(render, fr.x, fr.y + i * cellH, fr.x + fr.w, fr.y + i * cellH);
    }
}

void renderizarTablero(int** mat, int cf, SDL_Rect fr, SDL_Renderer* render)
{
    // 1. Limpiar la pantalla con un color de fondo (blanco)
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderClear(render);

    // 2. Dibujar la grilla del tablero
    inicializarTablero(cf, fr, render);

    // 3. Dibujar todas las fichas (X y O) que estén en la matriz
    int cellW = fr.w / cf;
    int cellH = fr.h / cf;

    for (int fila = 0; fila < cf; fila++)
    {
        for (int col = 0; col < cf; col++)
        {
            if (mat[fila][col] != 0)
            {
                SDL_Rect celda = { fr.x + col * cellW, fr.y + fila * cellH, cellW, cellH };
                if (mat[fila][col] == 1)
                {
                    dibujarX(render, celda);
                }
                else
                {
                    dibujarO(render, celda);
                }
            }
        }
    }

    // 4. Presentar el cuadro final y completo en la pantalla.
    // Esta es la ÚNICA llamada a SDL_RenderPresent por cada cuadro.
    SDL_RenderPresent(render);
}

void dibujarX(SDL_Renderer* render, SDL_Rect celda)
{
    SDL_SetRenderDrawColor(render, 255, 0, 0, 255); // Rojo para la X
    int grosor = 3;
    int margen = celda.w / 8;

    int x1 = celda.x + margen;
    int y1 = celda.y + margen;
    int x2 = celda.x + celda.w - margen;
    int y2 = celda.y + celda.h - margen;

    // Dibuja las dos líneas de la X
    for (int i = -grosor / 2; i <= grosor / 2; i++)
    {
        SDL_RenderDrawLine(render, x1 + i, y1, x2 + i, y2);
        SDL_RenderDrawLine(render, x1 + i, y2, x2 + i, y1);
    }
}

void dibujarO(SDL_Renderer* render, SDL_Rect celda)
{
    SDL_SetRenderDrawColor(render, 0, 0, 255, 255);
    int radio = celda.w / 4;
    int cx = celda.x + celda.w / 2;
    int cy = celda.y + celda.h / 2;

    int grosor = 3;

    for (int r = radio - grosor; r <= radio; r++)
    {
        for (int w = -r; w <= r; w++)
        {
            for (int h = -r; h <= r; h++)
            {
                if (w * w + h * h >= (r - 1) * (r - 1) && w * w + h * h <= r * r)
                {
                    SDL_RenderDrawPoint(render, cx + w, cy + h);
                }
            }
        }
    }
}

bool isMouseOver(SDL_Rect rect, int x, int y)
{
    return x >= rect.x && x <= rect.x + rect.w &&
           y >= rect.y && y <= rect.y + rect.h;
}

void manejarClick(SDL_Event e, int** mat, int cf, SDL_Rect fr, int* turno)
{
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
    {
        int mouseX = e.button.x;
        int mouseY = e.button.y;

        int cellWidth = fr.w / cf;
        int cellHeight = fr.h / cf;

        int col = (mouseX - fr.x) / cellWidth;
        int fila = (mouseY - fr.y) / cellHeight;

        if (fila >= 0 && fila < cf && col >= 0 && col < cf)
        {
            if (mat[fila][col] == 0)
            {
                mat[fila][col] = (*turno == 0) ? 1 : 2;
                *turno = (*turno + 1) % 2;
            }
        }
    }
}
