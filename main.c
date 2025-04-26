#include "sdl2.h"
#include "taceti.h"
#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int** mat, entrada = 0;
    mat = (int**)matrizCrear(sizeof(int), 3, 3);
    inicializarMatriz(mat, 3);
    int modo;
    int ganador;
    int turno = 0; // 0 = X, 1 = O
    unsigned char done = 0;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event e;
    SDL_Rect tableroRect = {270, 170, 300, 300};

    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL no se ha podido inicializar. SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("TaCeTi", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 840, 640, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!window || !renderer) {
        printf("No se pudo crear la ventana o renderer\n");
        SDL_Quit();
        return 1;
    }

    do {
        printf("Seleccione una opción:\n[1] Jugar\n[2] Ver Ranking\n[3] Salir\n");
        scanf("%d", &entrada);
        fflush(stdin);

        switch (entrada) {
            case 1: {
                    printf("Contra quien queres jugar\n[1] Otro jugador\n[2] Maquina\n");
                    scanf("%d", &modo);

                    inicializarTablero(3, tableroRect, renderer);
                    renderizarTablero(mat, 3, tableroRect, renderer);
                    turno = 0;

                    while (!done)
                    {
                        while (SDL_PollEvent(&e))
                        {
                            if (e.type == SDL_QUIT)
                                done = 1;
                            else if (modo == 1)
                            {
                                ganador = verificarGanador(mat);
                                done = verificar(ganador, mat);
                                manejarClick(e, mat, 3, tableroRect, &turno);
                            }
                            else if (modo == 2 && turno == 0)
                            {
                                manejarClick(e, mat, 3, tableroRect, &turno);
                                ganador = verificarGanador(mat);
                                done = verificar(ganador, mat);
                                if (turno == 1)
                                {
                                    ganador = verificarGanador(mat);
                                    done = verificar(ganador, mat);
                                    SDL_Delay(300); // simulamos que piensa
                                    jugarMaquina(mat, 3);
                                    turno = 0;
                                }
                            }
                        }
                        renderizarTablero(mat, 3, tableroRect, renderer);
                        SDL_Delay(16);
                    }
                    break;
}


            case 2:
                // verRanking();
                break;

            case 3:
                done = 1;
                break;
        }
    } while (!done);

    // Limpieza
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    mostrarMatriz(mat,3,3);
    matrizDestruir((void**)mat, 3);

    return 0;
}
