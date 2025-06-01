#include "arbol.h"
#include <string.h>
#include <stdlib.h>

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

    // Copiamos todo el contenido de info_src
    memcpy(nodo->info, info_src, sizeof(GameInfo));

    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}

/* -----------------------------------------------------------------
   2) destruirArbolJuego
     - Versión recursiva (post‐orden) para liberar hijos y hermanos
     - Patrón:
         if (raiz == NULL) return;
         destruirArbolJuego(raiz->izq);
         destruirArbolJuego(raiz->der);
         free(raiz->info);
         free(raiz);
   ----------------------------------------------------------------- */
void destruirArbolJuego(tNodoArbol *raiz)
{
    if (!raiz) return;

    // Primero destruyo toda la rama de hijos (izq)
    destruirArbolJuego(raiz->izq);
    // Luego destruyo la rama de hermanos (der)
    destruirArbolJuego(raiz->der);

    // Después libero el bloque de GameInfo
    free(raiz->info);
    // Finalmente libero el nodo mismo
    free(raiz);
}

/* -----------------------------------------------------------------
   3) expandirArbolJuego
      - Si el estado en “info” es terminal (ganador/empate), no crea hijos.
      - Si no, genera un hijo por cada casilla vacía:
          . El primer hijo se enlaza en nodo->izq.
          . Cada hijo sucesivo se enlaza a través de ->der (hermanos).
      - Una vez creados todos los hijos directos, recorre cada hijo
        y llama recursivamente para expandirlo.
   ----------------------------------------------------------------- */
void expandirArbolJuego(tNodoArbol *nodo)
{
    if (!nodo) return;
    GameInfo *info = (GameInfo*) nodo->info;

    // Construimos un arreglo local de punteros a cada fila del tablero,
    // para poder llamar a tus funciones comprobar ganador y empate:
    int* filas[3];
    for (int i = 0; i < 3; i++) {
        filas[i] = info->tablero[i];
    }

    // 1) Verifico si este estado ya es terminal
    int g = verificarGanador(filas);
    if (g != 0 || esEmpate(filas)) {
        // Gana X (1), gana O (2) o empate: no genero hijos
        return;
    }

    // 2) No es terminal: generar un hijo por cada casilla vacía.
    //    El “primer hijo” irá en nodo->izq, los demás se encadenarán por ->der.

    int jugadorActual = info->jugador;         // 1=X o 2=O (quien acaba de mover aquí)
    int siguienteJugador = 3 - jugadorActual;   // el que mueva en el siguiente nivel

    tNodoArbol *ultimoHermano = NULL;

    // Recorro las 9 casillas
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (info->tablero[i][j] == 0) {
                // 2.1) Construyo un GameInfo para el hijo, copiando tablero e info
                GameInfo hijoInfo;
                memcpy(hijoInfo.tablero, info->tablero, sizeof(hijoInfo.tablero));
                hijoInfo.jugador = siguienteJugador;
                hijoInfo.filaJugada = i;
                hijoInfo.colJugada = j;
                hijoInfo.valor = 0;

                // 2.2) Marco la jugada de “siguienteJugador” en (i,j)
                hijoInfo.tablero[i][j] = siguienteJugador;

                // 2.3) Creo el nodo hijo
                tNodoArbol *hijo = crearNodoJuego(&hijoInfo);
                if (!hijo) {
                    // En caso de falla de malloc, continuamos sin agregar este hijo
                    continue;
                }

                // 2.4) Enlazo el hijo en la lista de hijos de “nodo”
                if (nodo->izq == NULL) {
                    // Primer hijo
                    nodo->izq = hijo;
                    ultimoHermano = hijo;
                }
                else {
                    // Lo conecto como hermano derecho del último hijo
                    ultimoHermano->der = hijo;
                    ultimoHermano = hijo;
                }
            }
        }
    }

    // 3) Ahora recorro recursivamente cada hijo directo y lo expando
    tNodoArbol *cursor = nodo->izq;
    while (cursor) {
        expandirArbolJuego(cursor);
        cursor = cursor->der;  // siguiente hermano
    }
}

/* -----------------------------------------------------------------
   4) minimaxArbolJuego (post‐orden):
      - Si el nodo es terminal (ganador/empate), asigna valor = +10/-10/0.
      - Si no, recorre cada hijo (izq->der) recursivamente, luego:
          . Si info->jugador == 1 (X), busca el MÁXIMO valor entre hijos
          . Si info->jugador == 2 (O), busca el MÍNIMO valor entre hijos
        y asigna ese resultado a info->valor.
      - Retorna info->valor
   ----------------------------------------------------------------- */
int minimaxArbolJuego(tNodoArbol *nodo)
{
    if (!nodo) return 0;
    GameInfo *info = (GameInfo*) nodo->info;

    // Construimos el arreglo de punteros a fila para llamar a tus funciones
    int* filas[3];
    for (int i = 0; i < 3; i++) {
        filas[i] = info->tablero[i];
    }

    // 1) Caso base: si hay ganador o empate, asigno valor directamente
    int g = verificarGanador(filas);
    if (g == 2) {
        info->valor = +10;  // O ganó
        return info->valor;
    }
    if (g == 1) {
        info->valor = -10;  // X ganó
        return info->valor;
    }
    if (esEmpate(filas)) {
        info->valor = 0;    // Empate
        return info->valor;
    }

    // 2) No es hoja: recorro hijos en post‐orden
    tNodoArbol *hijo = nodo->izq;
    while (hijo) {
        minimaxArbolJuego(hijo);
        hijo = hijo->der;
    }

    // 3) Según “info->jugador” decido si debo MAXIMIZAR o MINIMIZAR
    int mejor;
    hijo = nodo->izq;
    if (info->jugador == 1) {
        // X busca el MÁXIMO
        mejor = -100000;
        while (hijo) {
            GameInfo *hinfo = (GameInfo*) hijo->info;
            if (hinfo->valor > mejor) {
                mejor = hinfo->valor;
            }
            hijo = hijo->der;
        }
    } else {
        // O busca el MÍNIMO
        mejor = +100000;
        while (hijo) {
            GameInfo *hinfo = (GameInfo*) hijo->info;
            if (hinfo->valor < mejor) {
                mejor = hinfo->valor;
            }
            hijo = hijo->der;
        }
    }

    info->valor = mejor;
    return info->valor;
}

/* -----------------------------------------------------------------
   5) elegirMejorParaO:
      - Entre los hijos directos de “raiz” (raiz->izq y sus hermanos),
        busca el que tenga info->valor mínimo (mejor para O).
      - Retorna NULL si raiz->izq es NULL.
   ----------------------------------------------------------------- */
tNodoArbol* elegirMejorParaO(tNodoArbol *raiz)
{
    if (!raiz) return NULL;
    tNodoArbol *hijo = raiz->izq;
    if (!hijo) return NULL;

    tNodoArbol *mejor = hijo;
    GameInfo *gmejor = (GameInfo*) hijo->info;
    int minVal = gmejor->valor;

    hijo = hijo->der;
    while (hijo) {
        GameInfo *hinfo = (GameInfo*) hijo->info;
        if (hinfo->valor < minVal) {
            minVal = hinfo->valor;
            mejor = hijo;
        }
        hijo = hijo->der;
    }
    return mejor;
}

/* -----------------------------------------------------------------
   6) elegirPeorParaO:
      - Entre los hijos directos de “raiz”, busca el que tenga el valor máximo
        (peor para O, mejor para X).
      - Retorna NULL si no hay hijos.
   ----------------------------------------------------------------- */
tNodoArbol* elegirPeorParaO(tNodoArbol *raiz)
{
    if (!raiz) return NULL;
    tNodoArbol *hijo = raiz->izq;
    if (!hijo) return NULL;

    tNodoArbol *peor = hijo;
    GameInfo *gpeor = (GameInfo*) hijo->info;
    int maxVal = gpeor->valor;

    hijo = hijo->der;
    while (hijo) {
        GameInfo *hinfo = (GameInfo*) hijo->info;
        if (hinfo->valor > maxVal) {
            maxVal = hinfo->valor;
            peor = hijo;
        }
        hijo = hijo->der;
    }
    return peor;
}
