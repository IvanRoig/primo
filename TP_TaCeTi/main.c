#include "sdl2.h"
#include "taceti.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "lista.h"
#include "api.h"
#include "config.h"
#include "arbol.h"

int main(int argc, char *argv[])
{
    tConfig cfg;
    if (!leerConfig("config.ini", &cfg))
    {
        fprintf(stderr, "Error cargando config.ini\n");
        return 1;
    }
    printf("Modo: %s  Partidas/PJ: %d\n", cfg.modo, cfg.partidas);

    jugador jug;
    tLista listaJugadores;
    crearLista(&listaJugadores);

    char nombre[50];
    int** mat = (int**) matrizCrear(sizeof(int), 3, 3);
    inicializarMatriz(mat, 3);

    int cantJugadores, ganador, turno = 0;
    unsigned char done = 0;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event e;
    SDL_Rect tableroRect = {270, 170, 300, 300};

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL no se ha podido inicializar. SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("TaCeTi", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 840, 640, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!window || !renderer)
    {
        printf("No se pudo crear la ventana o renderer\n");
        SDL_Quit();
        return 1;
    }

    int entrada = 0;
    do
    {
        printf("Seleccione una opcion:\n[1] Jugar\n[2] Ver Ranking\n[3] Salir\n");
        scanf("%d", &entrada);
        fflush(stdin);

        switch (entrada)
        {
        case 1:
        {
            printf("Ingrese la cantidad de jugadores\n");
            scanf("%d", &cantJugadores);
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);

            for (int i = 0; i < cantJugadores; i++)
            {
                printf("Ingrese el nombre del jugador %d \n", i + 1);
                if (fgets(nombre, sizeof(nombre), stdin) != NULL)
                {
                    nombre[strcspn(nombre, "\n")] = '\0';
                    strcpy(jug.nombre, nombre);
                    jug.puntaje = 0;
                    ponerAlFinal(&listaJugadores, &jug, sizeof(jug));
                }
            }

            printf("\n--- Jugadores Cargados ---\n");
            mostrarLista(&listaJugadores, mostrarJugador);

            srand((unsigned)time(NULL));
            tLista listaTemp;
            crearLista(&listaTemp);

            unsigned n = contarLista(&listaJugadores);
            while (n-- > 0)
            {
                jugador jugAux;
                unsigned pos = rand() % (n + 1);
                sacarEnPos(&listaJugadores, &jugAux, sizeof(jugAux), pos);
                ponerAlFinal(&listaTemp, &jugAux, sizeof(jugAux));
            }

            printf("\n--- Orden de Turnos ---\n");
            mostrarLista(&listaTemp, mostrarJugador);

            tLista listaRanking;
            crearLista(&listaRanking);

            jugador jugActual;
            while (sacarPrimero(&listaTemp, &jugActual, sizeof(jugActual)))
            {
                printf("\n--- Turno de %s ---\n", jugActual.nombre);
                for (int p = 0; p < cfg.partidas; p++)
                {
                    printf("\n--- Partida %d de %s ---\n", p + 1, jugActual.nombre);
                    inicializarMatriz(mat, 3);
                    //inicializarTablero(3, tableroRect, renderer);
                    renderizarTablero(mat, 3, tableroRect, renderer);

                    turno = rand() % 2;
                    printf("Empieza la partida: %s\n",
                           (turno == 0) ? jugActual.nombre : "Maquina (O)");
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
                            }
                            else
                            {
                                SDL_Delay(200);
                                GameInfo rootInfo;
                                for (int i = 0; i < 3; i++)
                                    for (int j = 0; j < 3; j++)
                                        rootInfo.tablero[i][j] = mat[i][j];
                                rootInfo.jugador = 2; // La máquina es el jugador 2 ('O')

                                rootInfo.filaJugada = -1;
                                rootInfo.colJugada = -1;
                                rootInfo.valor = 0;

                                tNodoArbol *raiz = crearNodoJuego(&rootInfo);
                                expandirArbolJuego(raiz);
                                minimaxArbolJuego(raiz);

                                tNodoArbol *seleccion = NULL;
                                if (strcmp(cfg.modo, "FACIL") == 0)
                                {
                                    seleccion = elegirPeorParaO(raiz);
                                }
                                else if (strcmp(cfg.modo, "IMPOSIBLE") == 0)
                                {
                                    seleccion = elegirMejorParaO(raiz);
                                }
                                else
                                {
                                    // Modo intermedio: jugada aleatoria de la máquina
                                    jugarMaquina(mat, 3);
                                    turno = 0;
                                    destruirArbolJuego(raiz);
                                    break;
                                }

                                if (seleccion)
                                {
                                    GameInfo *chosen = (GameInfo*) seleccion->info;
                                    mat[chosen->filaJugada][chosen->colJugada] = 2;
                                }

                                destruirArbolJuego(raiz);
                                turno = 0;
                            }

                            ganador = verificarGanador(mat);
                            done = verificar(ganador, mat);
                        }
                        renderizarTablero(mat, 3, tableroRect, renderer);
                        SDL_Delay(16);
                    }

                    if (done == 1)      jugActual.puntaje += 3;
                    else if (done == 2) jugActual.puntaje -= 1;
                    else                jugActual.puntaje += 2;
                }

                ponerAlFinal(&listaRanking, &jugActual, sizeof(jugActual));
            }

            listaJugadores = listaRanking;
            mostrarLista(&listaJugadores, mostrarJugador);
            enviarResultados(&listaJugadores);

            vaciarLista(&listaJugadores);
            break;
        }

        case 2:
            hacerGET();
            break;

        case 3:
            done = 1;
            break;
        }
    }
    while (!done || entrada != 3);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    mostrarMatriz(mat, 3, 3);
    matrizDestruir((void**)mat, 3);
    return 0;
}
