#ifndef TACETI_H_INCLUDED
#define TACETI_H_INCLUDED

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sdl2.h"
#include "lista.h"

typedef struct
{
    char nombre[50];
    int puntaje;
}jugador;

typedef struct
{
    int fila, col;
} tMovimiento;

void inicializarMatriz(int** mat, int cf);
void mostrarMatriz(int** mat, int cf, int cc);
void** matrizCrear(size_t tamElem, int filas, int columnas);
void matrizDestruir(void** mat,int filas);
void jugarMaquina(int** mat, int cf);
int verificarGanador(int** mat);
int esEmpate(int** mat);
int verificar(int ganador, int** mat);
void mostrarJugador(void *);
int encontrarMaximoPuntaje(tLista *listaJugadores);

// Informe
FILE* headerArchivo(FILE* archivo);
void footerArchivo(tLista *l,FILE* archivo);
void cargarInforme(FILE *informe, jugador jugActual, int** matriz, unsigned char estadoJuego);

#endif // TACETI_H_INCLUDED
