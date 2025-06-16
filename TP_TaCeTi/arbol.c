#include "arbol.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h> // Para INT_MAX y INT_MIN

#include "taceti.h" // Asegúrate de incluir esto para verificarGanador y esEmpate

/* -----------------------------------------------------------------
   1) crearNodoJuego
      - Reserva un tNodoArbol
      - Reserva un bloque GameInfo, copia *info_src en él
      - Inicializa izq=der=NULL
      - Retorna el nodo o NULL si falla malloc
   ----------------------------------------------------------------- */
tNodoArbol* crearNodoJuego(const GameInfo *info_src)
{
    tNodoArbol *nodo = (tNodoArbol*) malloc(sizeof(tNodoArbol));
    if (!nodo) return NULL;

    nodo->info = malloc(sizeof(GameInfo));
    if (!(nodo->info)) {
        free(nodo);
        return NULL;
    }

    memcpy(nodo->info, info_src, sizeof(GameInfo));

    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}

/* -----------------------------------------------------------------
   2) destruirArbolJuego
      - Versión recursiva (post‐orden) para liberar hijos y hermanos
   ----------------------------------------------------------------- */
void destruirArbolJuego(tNodoArbol *raiz)
{
    if (!raiz) return;
    destruirArbolJuego(raiz->izq);
    destruirArbolJuego(raiz->der);
    free(raiz->info);
    free(raiz);
}

/* -----------------------------------------------------------------
   3) expandirArbolJuego
      - Si el estado en “info” es terminal (ganador/empate), no crea hijos.
      - Si no, genera un hijo por cada casilla vacía.
   ----------------------------------------------------------------- */
void expandirArbolJuego(tNodoArbol *nodo)
{
    if (!nodo) return;
    GameInfo *info = (GameInfo*) nodo->info;

    // Para poder llamar a tus funciones verificarGanador y esEmpate,
    // necesitamos un int** a partir del tablero del GameInfo.
    int* filas[3];
    for (int i = 0; i < 3; i++) {
        filas[i] = info->tablero[i];
    }

    // 1) Verifico si este estado ya es terminal
    if (verificarGanador(filas) != 0 || esEmpate(filas)) {
        return; // Es un estado final, no se expande más.
    }

    // 2) No es terminal: generar un hijo por cada casilla vacía.
    tNodoArbol *ultimoHermano = NULL;
    int jugadorActual = info->jugador;
    int siguienteJugador = (jugadorActual == 1) ? 2 : 1;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (info->tablero[i][j] == 0) { // Si la casilla está libre
                // 2.1) Construyo un GameInfo para el hijo
                GameInfo hijoInfo;
                memcpy(hijoInfo.tablero, info->tablero, sizeof(hijoInfo.tablero));

                // --- ¡CORRECCIÓN CLAVE! ---
                // La jugada que se prueba es la del JUGADOR ACTUAL (info->jugador),
                // no la del siguiente.
                hijoInfo.tablero[i][j] = jugadorActual;

                // El turno en el nodo hijo será del SIGUIENTE jugador.
                hijoInfo.jugador = siguienteJugador;
                hijoInfo.filaJugada = i;
                hijoInfo.colJugada = j;
                hijoInfo.valor = 0;

                // 2.2) Creo y enlazo el nodo hijo
                tNodoArbol *hijo = crearNodoJuego(&hijoInfo);
                if (!hijo) continue;

                if (nodo->izq == NULL) {
                    nodo->izq = hijo;
                    ultimoHermano = hijo;
                } else {
                    ultimoHermano->der = hijo;
                    ultimoHermano = hijo;
                }
            }
        }
    }

    // 3) Expando recursivamente cada hijo generado
    tNodoArbol *cursor = nodo->izq;
    while (cursor) {
        expandirArbolJuego(cursor);
        cursor = cursor->der;
    }
}


/* -----------------------------------------------------------------
   4) minimaxArbolJuego (post‐orden):
      - Si el nodo es terminal, asigna valor (+10, -10, 0).
      - Si no, calcula el valor basado en los hijos (MAX si es turno de 'O', MIN si es de 'X').
   ----------------------------------------------------------------- */
int minimaxArbolJuego(tNodoArbol *nodo)
{
    if (!nodo) return 0;
    GameInfo *info = (GameInfo*) nodo->info;

    // Creamos un int** para usar las funciones de verificación
    int* filas[3];
    for (int i = 0; i < 3; i++) {
        filas[i] = info->tablero[i];
    }

    // 1) Caso base: el nodo es terminal (hay un resultado final)
    int g = verificarGanador(filas);
    if (g == 2) { info->valor = 10; return info->valor; } // Gana O (máquina)
    if (g == 1) { info->valor = -10; return info->valor; } // Gana X (humano)
    if (esEmpate(filas)) { info->valor = 0; return info->valor; } // Empate

    // Si no tiene hijos, es una hoja del árbol. Su valor ya fue calculado arriba.
    if (!nodo->izq) return info->valor;

    // 2) Caso recursivo: no es terminal. Primero, calculamos el valor de todos los hijos.
    tNodoArbol *hijo = nodo->izq;
    while (hijo) {
        minimaxArbolJuego(hijo);
        hijo = hijo->der;
    }

    // 3) Ahora, calculamos el valor del NODO ACTUAL basado en los hijos.
    hijo = nodo->izq; // Reiniciamos el puntero al primer hijo

    if (info->jugador == 2) { // Es el turno de 'O' (máquina), el jugador MAXIMIZADOR
        int mejorValor = INT_MIN;
        while (hijo) {
            GameInfo *hinfo = (GameInfo*) hijo->info;
            if (hinfo->valor > mejorValor) {
                mejorValor = hinfo->valor;
            }
            hijo = hijo->der;
        }
        info->valor = mejorValor;
    } else { // Es el turno de 'X' (humano), el jugador MINIMIZADOR
        int peorValor = INT_MAX;
        while (hijo) {
            GameInfo *hinfo = (GameInfo*) hijo->info;
            if (hinfo->valor < peorValor) {
                peorValor = hinfo->valor;
            }
            hijo = hijo->der;
        }
        info->valor = peorValor;
    }

    return info->valor;
}


/* -----------------------------------------------------------------
   5) elegirMejorParaO:
      - Entre los hijos directos de “raiz”, busca el que tenga el valor MÁXIMO.
      - Esto representa la mejor jugada para 'O' (la máquina).
   ----------------------------------------------------------------- */
tNodoArbol* elegirMejorParaO(tNodoArbol *raiz)
{
    if (!raiz || !raiz->izq) return NULL;

    tNodoArbol *mejorJugadaNodo = NULL;
    int mejorValor = INT_MIN;

    tNodoArbol *hijoActual = raiz->izq;
    while (hijoActual) {
        GameInfo *hinfoActual = (GameInfo*) hijoActual->info;
        // 'O' (máquina) busca maximizar su puntuación. Elige el hijo con el valor más alto.
        if (hinfoActual->valor > mejorValor) {
            mejorValor = hinfoActual->valor;
            mejorJugadaNodo = hijoActual;
        }
        hijoActual = hijoActual->der;
    }
    return mejorJugadaNodo;
}

/* -----------------------------------------------------------------
   6) elegirPeorParaO:
      - Entre los hijos directos de “raiz”, busca el que tenga el valor MÍNIMO.
      - Esto representa la peor jugada para 'O' (modo fácil).
   ----------------------------------------------------------------- */
tNodoArbol* elegirPeorParaO(tNodoArbol *raiz)
{
    if (!raiz || !raiz->izq) return NULL;

    tNodoArbol *peorJugadaNodo = NULL;
    int peorValor = INT_MAX;

    tNodoArbol *hijoActual = raiz->izq;
    while (hijoActual) {
        GameInfo *hinfoActual = (GameInfo*) hijoActual->info;
        // Para jugar "mal", 'O' elige la jugada que le da el peor resultado (el valor más bajo).
        if (hinfoActual->valor < peorValor) {
            peorValor = hinfoActual->valor;
            peorJugadaNodo = hijoActual;
        }
        hijoActual = hijoActual->der;
    }
    return peorJugadaNodo;
}
