#ifndef TACETI_H_INCLUDED
#define TACETI_H_INCLUDED
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sdl2.h"

typedef struct
{
    char nombre[50];
    int puntaje;
}jugador;

//Pueden llegar a servir
int contarCelulas(int** mat, int filas, int columnas, int fila, int columna);
void actualizarCelula(int** mat, int contador, int x, int y);
void estadoActual(int** mat, int cf, int cc);
//void estadoFuturo(int** mat,int cf, int cc,SDL_Rect fr, SDL_Renderer* render);


//Se usan
void inicializarMatriz(int** mat, int cf);
void mostrarMatriz(int** mat, int cf, int cc);
void** matrizCrear(size_t tamElem, int filas, int columnas);
void matrizDestruir(void** mat,int filas);
void jugarMaquina(int** mat, int cf);
int verificarGanador(int** mat);
int esEmpate(int** mat);
int verificar(int ganador, int** mat);
void mostrarJugador(void *);



#endif // TACETI_H_INCLUDED
