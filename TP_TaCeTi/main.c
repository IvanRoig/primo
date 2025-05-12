#include "sdl2.h"
#include "taceti.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include "lista.h"
#include "api.h"


int main(int argc, char *argv[])
{
    jugador jug;
    tLista listaJugadores;
    crearLista(&listaJugadores);
    char nombre[50];
    int** mat, entrada = 0;
    mat = (int**)matrizCrear(sizeof(int), 3, 3);
    inicializarMatriz(mat, 3);
    int cantJugadores;
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
        printf("Seleccione una opcion:\n[1] Jugar\n[2] Ver Ranking\n[3] Salir\n");
        scanf("%d", &entrada);
        fflush(stdin);
        //int entrada = mostrarMenu(renderer);

        switch (entrada) {
            case 1: {
                    printf("Ingrese la cantidad de jugadores\n");
                    scanf("%d", &cantJugadores);

                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF);
                    for (int i = 0; i < cantJugadores; i++)
                    {
                        printf("Ingrese el nombre del jugador %d \n", i + 1);
                        if (fgets(nombre, sizeof(nombre), stdin) != NULL)
                        {
                            nombre[strcspn(nombre, "\n")] = '\0'; // eliminar salto de línea
                            strcpy(jug.nombre, nombre);
                            jug.puntaje = 0;
                            ponerAlComienzo(&listaJugadores, &jug, sizeof(jug));
                        }
                        else
                        {
                            printf("Error al leer el nombre.\n");
                        }
                    }

                    printf("---Jugadores Cargados--- \n");
                    mostrarLista(&listaJugadores, mostrarJugador);
                    inicializarTablero(3, tableroRect, renderer);
                    renderizarTablero(mat, 3, tableroRect, renderer);
                    turno = 0;

                    tNodo* actual = listaJugadores;
                    while (actual != NULL)
                    {
                        jugador* j = (jugador*)actual->dato;

                        printf("\n--- Turno de %s ---\n", j->nombre);
                        inicializarMatriz(mat, 3);
                        inicializarTablero(3, tableroRect, renderer);
                        renderizarTablero(mat, 3, tableroRect, renderer);
                        turno = 0;
                        done = 0;

                        while (!done)
                        {
                            while (SDL_PollEvent(&e))
                            {
                                if (e.type == SDL_QUIT)
                                {
                                    done = 1;
                                    break;
                                }
                                if (turno == 0)
                                {
                                    manejarClick(e, mat, 3, tableroRect, &turno);
                                    ganador = verificarGanador(mat);
                                    done = verificar(ganador, mat);
                                }
                                if (!done && turno == 1)
                                {
                                    SDL_Delay(500); // la máquina "piensa"
                                    jugarMaquina(mat, 3);
                                    turno = 0;
                                    ganador = verificarGanador(mat);
                                    done = verificar(ganador, mat);
                                }
                            }

                            renderizarTablero(mat, 3, tableroRect, renderer);
                            SDL_Delay(16);
                        }
                        if(done == 1)
                            j->puntaje += 3;
                        else
                            j->puntaje += 1;

                        actual = actual->sig;
                    }
                    mostrarLista(&listaJugadores, mostrarJugador);
                    enviarResultados(&listaJugadores);
                    break;
            }


            case 2:
                hacerGET();
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
