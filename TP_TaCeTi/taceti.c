#include "taceti.h"

void inicializarMatriz(int** mat, int cf)
{
    int i, j;
    for(i=0; i<cf; i++)
    {
        for(j=0; j<cf; j++)
            mat[i][j] = 0;
    }
}

void mostrarMatriz(int** mat, int cf, int cc)
{
    int i, j;
    for(i=0; i<cf; i++)
    {
        for(j=0; j<cc; j++)
        {
            printf("%d  ",mat[i][j]);
        }
        printf("\n");
    }
}

void** matrizCrear(size_t tamElem, int filas, int columnas)
{
    int contadorDeInserts = 1;// cuenta la cantidad de filas que inserta en cada direccion para liberar solo las que reservo en cada de poder reservar la matriz entera
    void** mat = malloc(sizeof(void*) * filas);
    if(!mat)
        return NULL;
    void** ult = mat + filas-1;
    for(void** i = mat; i <= ult; i++)
    {
        *i = malloc(tamElem * columnas);
        if(!(*i))
        {
            matrizDestruir(mat,contadorDeInserts);
            return NULL;
        }
        contadorDeInserts++;
    }

    return mat;
}

void matrizDestruir(void** mat,int filas)
{
    void** ult = mat + filas-1;
    for(void** i = mat; i <= ult; i++)
    {
        free(*i);
    }
    free(mat);
}

void jugarMaquina(int** mat, int cf)
{
    // Inicializar aleatorio solo una vez
    static int inicializado = 0;
    if (!inicializado)
    {
        srand(time(NULL));
        inicializado = 1;
    }

    int libres[9][2];
    int cantLibres = 0;

    for (int i = 0; i < cf; i++)
    {
        for (int j = 0; j < cf; j++)
        {
            if (mat[i][j] == 0)
            {
                libres[cantLibres][0] = i;
                libres[cantLibres][1] = j;
                cantLibres++;
            }
        }
    }

    if (cantLibres > 0)
    {
        int r = rand() % cantLibres;
        int fila = libres[r][0];
        int col = libres[r][1];
        mat[fila][col] = 2; // Jugada de la máquina (O)
    }
}

int verificarGanador(int** mat)
{
    for (int i = 0; i < 3; i++)
    {
        // Filas
        if (mat[i][0] != 0 && mat[i][0] == mat[i][1] && mat[i][1] == mat[i][2])
            return mat[i][0];
        // Columnas
        if (mat[0][i] != 0 && mat[0][i] == mat[1][i] && mat[1][i] == mat[2][i])
            return mat[0][i];
    }

    // Diagonales
    if (mat[0][0] != 0 && mat[0][0] == mat[1][1] && mat[1][1] == mat[2][2])
        return mat[0][0];
    if (mat[0][2] != 0 && mat[0][2] == mat[1][1] && mat[1][1] == mat[2][0])
        return mat[0][2];

    return 0; // No hay ganador
}

int esEmpate(int** mat)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (mat[i][j] == 0)
                return 0;

    return 1;
}

int verificar(int ganador, int** mat)
{
    int done = 0;
    if (ganador == 1)
    {
        printf("Gano el jugador (X)!\n");
        done = 1;
    }
    else if(ganador == 2)
    {
        printf("Gano la maquina (O)!\n");
        done = 2;
    }
    else if(esEmpate(mat))
    {
        printf("Empate!\n");
        done = 3;
    }
    return done;
}

void mostrarJugador(void *dato)
{
    jugador *nombre = (jugador *)dato;
    printf("Nombre: %s\tPuntaje: %d\n", nombre->nombre, nombre->puntaje);
}

int encontrarMaximoPuntaje(tLista *listaJugadores)
{
    if (listaVacia(listaJugadores)) {
        return 0;
    }

    int maxPuntaje = 0;
    tNodo* actual = *listaJugadores;

    while (actual != NULL)
    {
        jugador* jug = (jugador*)actual->dato;
        if (jug->puntaje > maxPuntaje)
        {
            maxPuntaje = jug->puntaje;
        }
        actual = actual->sig;
    }
    return maxPuntaje;
}

FILE* headerArchivo(FILE* archivo)
{
    time_t t;
    struct tm *tm_info;
    char nombreArchivo[30];

    time(&t);
    tm_info = localtime(&t);
    strftime(nombreArchivo, sizeof(nombreArchivo), "%Y-%m-%d-%H-%M.txt", tm_info);

    archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL)
    {
        fprintf(stderr, "Error: No se pudo crear el archivo de informe '%s'.\n", nombreArchivo);
        return 0;
    }

    fprintf(archivo, "========================================\n");
    fprintf(archivo, "    INFORME DE RESULTADOS - Ta-Ce-Ti\n");
    fprintf(archivo, "========================================\n\n");
    return archivo;
}

void cargarInforme(FILE *informe, jugador jugActual, int** matriz, unsigned char estadoJuego) {
    if (informe == NULL) {
        printf("Error: El puntero al archivo es NULL. No se pudo escribir el informe.\n");
        return;
    }

    fprintf(informe, "JUGADOR: %s\n", jugActual.nombre);
    fprintf(informe, "-TABLERO:\n");
    for (int i = 0; i < 3; i++) {
        fprintf(informe, "   "); // Para indentar el tablero
        for (int j = 0; j < 3; j++) {
            fprintf(informe, "%d ", matriz[i][j]);
        }
        fprintf(informe, "\n");
    }

    if(estadoJuego == 1)
    {
        fprintf(informe, "-GANADOR : \"%s\"\n", jugActual.nombre);
        fprintf(informe, "-PUNTAJE: 3\n");
    }
    else if(estadoJuego == 3)
    {
        fprintf(informe, "-EMPATE\n");
        fprintf(informe, "-PUNTAJE: 2\n");
    }
    else
    {
        fprintf(informe, "-GANO LA MAQUINA\n");
        fprintf(informe, "-PUNTAJE: -1\n");
    }
    fprintf(informe, "\n"); // Línea en blanco para separar entradas en el informe
}

void footerArchivo(tLista *l, FILE* archivo)
{
    fprintf(archivo, "--- PUNTAJE TOTAL POR JUGADOR ---\n");
    if (listaVacia(l))
        fprintf(archivo, "No hay datos de jugadores para mostrar.\n");
    else
    {
        tNodo* actual = *l;
        while (actual != NULL)
        {
            jugador* jug = (jugador*)actual->dato;
            fprintf(archivo, " > Jugador: %s\n", jug->nombre);
            fprintf(archivo, "   Puntaje Final: %d\n\n", jug->puntaje);
            actual = actual->sig;
        }
    }

    fprintf(archivo, "\n--- RESULTADO FINAL ---\n");
    int maxPuntaje = encontrarMaximoPuntaje(l);
    fprintf(archivo, "El mayor puntaje obtenido fue: %d\n\n", maxPuntaje);
    fprintf(archivo, "Jugador(es) con el mayor puntaje:\n");

    //recorro de nuevo para obtener el mayor puntaje
    tNodo* actual = *l;
    while (actual != NULL)
    {
        jugador* jug = (jugador*)actual->dato;
        if (jug->puntaje == maxPuntaje)
        {
            fprintf(archivo, "   - %s\n", jug->nombre);
        }
        actual = actual->sig;
    }

    fclose(archivo);
}
