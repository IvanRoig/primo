#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "taceti.h"

typedef struct sNodo
{
    void* dato;
    unsigned tam;
    struct sNodo *sig;
}tNodo;

typedef tNodo *tLista;

void crearLista(tLista *l);
int listaVacia(const tLista* l);
int listaLlena(tLista *l, unsigned tam);
void vaciarLista(tLista *l);
int ponerAlComienzo(tLista *l, void* dato, unsigned tam);
int sacarPrimero(tLista *l, void *dato, unsigned tam);
int verPrimero(const tLista* l, void* dato, unsigned tam);
int ponerAlFinal(tLista *l, void* dato, unsigned tam);
int sacarAlFinal(tLista *l, void* dato, unsigned tam);
int verUltimo(tLista *l, void* dato, unsigned tam);

int mostrarLista(tLista *l, void (*mostrarJugador)(void*));
unsigned contarLista(const tLista *l);

// extrae el nodo en la posición pos (0..n-1)
int sacarEnPos(tLista *l, void *dato, unsigned tam, unsigned pos);



#endif // LISTA_H_INCLUDED
