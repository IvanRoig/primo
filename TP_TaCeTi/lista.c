#include "lista.h"


void crearLista(tLista *l)
{
    *l = NULL;
}

int listaVacia(const tLista* l)
{
    return *l == NULL;
}

int listaLlena(tLista *l, unsigned tam)
{
    tNodo *aux = (tNodo*)malloc(sizeof(tNodo));
    void* info = malloc(tam);
    free(aux);
    free(info);
    return aux == NULL || info == NULL;
}

void vaciarLista(tLista *l)
{
    while(*l)
    {
        tNodo *aux = *l;
        *l = aux->sig;
        free(aux->dato);
        free(aux);
    }
}

int ponerAlComienzo(tLista *l, void* dato, unsigned tam)
{
    tNodo *nue = (tNodo*)malloc(sizeof(tNodo));
    if(!nue)
        return 0;
    nue->dato = malloc(tam);
    memcpy(nue->dato, dato, tam);
    nue->tam = tam;
    nue->sig = *l;
    *l = nue;
    return 1;
}

int sacarPrimero(tLista *l, void* dato, unsigned tam)
{
    tNodo *aux = *l;
    if(listaVacia(l))
        return 0;
    *l = aux->sig;
    memcpy(dato, aux->dato, tam);
    free(aux->dato);
    free(aux);
    return 1;
}

int verPrimero(const tLista* l, void* dato, unsigned tam)
{
    if(listaVacia(l))
        return 0;
    memcpy(dato, (*l)->dato, tam);
    return 1;
}

int ponerAlFinal(tLista *l, void* dato, unsigned tam)
{
    tNodo *nue;
    while(*l)
        l = &(*l)->sig;
    nue = (tNodo*)malloc(sizeof(tNodo));
    if(!nue)
        return 0;
    nue->dato = malloc(tam);
    nue->tam = tam;
    nue->sig = NULL;
    *l = nue;
    return 1;
}

int sacarAlFinal(tLista *l, void* dato, unsigned tam)
{
    if(listaVacia(l))
        return 0;
    while((*l)->sig)
        l =&(*l)->sig;
    memcpy(dato, (*l)->dato, tam);
    free((*l)->dato);
    free(*l);
    *l = NULL;
    return 1;
}

int verUltimo(tLista *l, void* dato, unsigned tam)
{
    if(listaVacia(l))
        return 0;
    while((*l)->sig)
        l = &(*l)->sig;
    memcpy(dato, (*l)->dato, tam);
    return 1;
}

int mostrarLista(tLista *l, void (*mostrar)(void*))
{
    if(*l == NULL)
        return 0;
    mostrarLista(&(*l)->sig, mostrar);
    mostrar((*l)->dato);
    return 1;
}





