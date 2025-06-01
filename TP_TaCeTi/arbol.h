#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include "taceti.h"

typedef struct {
    int tablero[3][3];
    int jugador;      // 1 = X, 2 = O
    int filaJugada;   // fila (0..2) que llevó a este nodo
    int colJugada;    // col  (0..2) que llevó a este nodo
    int valor;        // puntuación Minimax para este nodo
} GameInfo;

typedef struct sNodoArbol {
    void *info;               // apunta a GameInfo*
    struct sNodoArbol *izq;   // primer hijo (primer movimiento)
    struct sNodoArbol *der;   // hermano derecho (siguiente movimiento posible)
} tNodoArbol;

/**
 * Crea un tNodoArbol cuyo “info” es un bloque dinámico GameInfo (se hace malloc y copia).
 *  - info_src: puntero a la estructura GameInfo que queremos clonar dentro del nodo.
 *  - Devuelve NULL si falla malloc; o la dirección de un nodo con izq=der=NULL, info=(GameInfo*).
 */
tNodoArbol* crearNodoJuego(const GameInfo *info_src);

/**
 * Expande recursivamente TODO el árbol de juego de Ta-Te-Ti a partir de “nodo”:
 *   - Si el estado en “nodo->info” ya es terminal (ganador o empate), deja el nodo sin hijos.
 *   - En caso contrario, genera un hijo por cada casilla vacía:
 *       * El primer hijo se enlaza en nodo->izq.
 *       * El resto de hijos se encadenan entre sí utilizando ->der (como “hermanos”).
 */
void expandirArbolJuego(tNodoArbol *nodo);

/**
 * Aplica Minimax sobre el árbol ya expandido:
 *   - Si “nodo” es hoja (ganador o empate), asigna info->valor = +10 (si O ganó),
 *     -10 (si X ganó) o 0 (si empate).
 *   - Si no es hoja, primero recorre a todos los hijos (izq + der en post‐orden),
 *     luego, según “info->jugador” en ese nodo:
 *       * Si jugador==1 (X), busca el MÁXIMO valor entre hijos y lo asigna a info->valor.
 *       * Si jugador==2 (O), busca el MÍNIMO valor entre hijos y lo asigna a info->valor.
 *   - Devuelve el valor asignado a “nodo->info->valor”.
 */
int minimaxArbolJuego(tNodoArbol *nodo);

/**
 * Entre los hijos directos de “raiz” (raiz->izq, raíz->izq->der, etc.),
 * elige el que tenga el valor **mínimo** (mejor para O), retornando su puntero.
 * Devuelve NULL si no hay hijos.
 */
tNodoArbol* elegirMejorParaO(tNodoArbol *raiz);

/**
 * Entre los hijos directos de “raiz”, elige el que tenga el valor **máximo**
 * (peor para O, mejor para X), retornando su puntero.
 * Devuelve NULL si no hay hijos.
 */
tNodoArbol* elegirPeorParaO(tNodoArbol *raiz);

/**
 * Libera RECURSIVAMENTE TODO el árbol (izq y der), luego libera el bloque de info (GameInfo*),
 * y finalmente el nodo.
 * El orden de destrucción es exactamente el mismo patrón que en el PDF “Arboles_ArbolBinarioDeBusqueda.pdf”.
 */
void destruirArbolJuego(tNodoArbol *raiz);

#endif // ARBOL_H_INCLUDED
