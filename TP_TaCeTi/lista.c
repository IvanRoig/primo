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
    while (*l)
        l = &(*l)->sig;
    tNodo *nue = malloc(sizeof(tNodo));
    if (!nue) return 0;
    nue->dato = malloc(tam);
    if (!nue->dato)
    {
        free(nue);
        return 0;
    }
    memcpy(nue->dato, dato, tam);
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

int mostrarLista(tLista *l, void (*mostrar)(void *))
{
    tNodo *act = *l;
    if (!act) return 0;
    while (act)
    {
        mostrar(act->dato);
        act = act->sig;
    }
    return 1;
}


// cuenta cuántos nodos hay
unsigned contarLista(const tLista *l)
{
    unsigned cnt = 0;
    tNodo *act = *l;
    while (act)
    {
        cnt++;
        act = act->sig;
    }
    return cnt;
}

// saca el nodo en la posición pos; usa sacarPrimero si pos == 0
int sacarEnPos(tLista *l, void *dato, unsigned tam, unsigned pos)
{
    if (listaVacia(l)) return 0;
    if (pos == 0)
    {
        return sacarPrimero(l, dato, tam);
    }
    // buscamos el nodo anterior al que queremos extraer
    tNodo *prev = *l;
    for (unsigned i = 0; i + 1 < pos; i++)
    {
        if (!prev->sig) return 0;  // posición fuera de rango
        prev = prev->sig;
    }
    tNodo *elim = prev->sig;
    if (!elim) return 0;
    prev->sig = elim->sig;
    memcpy(dato, elim->dato, tam);
    free(elim->dato);
    free(elim);
    return 1;
}




