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

void estadoActual(int** mat, int cf, int cc)
{
    int x=0,y=0, contador=0;

    for(x=0; x<cf; x++)//filas
        {
            for(y=0; y<cc; y++)//columnas
            {
                contador = contarCelulas(mat,cf,cc,x,y);
                actualizarCelula(mat,contador,x,y);
            }
        }
}

int contarCelulas(int** matriz, int filas, int columnas, int fila, int columna)
{
    int nuevaFila, nuevaColumna,i;
    int contador = 0;

    int direcciones[8][2] =
    {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},          {0, 1},
        {1, -1}, {1, 0}, {1, 1}
    };

    for (i = 0; i < 8; i++)
    {
        nuevaFila = fila + direcciones[i][0];
        nuevaColumna = columna + direcciones[i][1];

        if (nuevaFila >= 0 && nuevaFila < filas && nuevaColumna >= 0 && nuevaColumna < columnas)
        {
            if (matriz[nuevaFila][nuevaColumna] == 254 || matriz[nuevaFila][nuevaColumna] == 5 || matriz[nuevaFila][nuevaColumna] == 6)
                contador++;
        }
    }

    return contador;
}

void actualizarCelula(int** mat, int contador, int x, int y)
{
    if(mat[x][y]== 32 && (contador!=3))
        mat[x][y] = 3; //es gion y muere
    if(mat[x][y]== 32 && (contador ==3))
        mat[x][y] = 4; //es gion y nace
    if(mat[x][y] == 254 && (contador <=1 || contador>3))
        mat[x][y] = 5; //es o y muere
    if(mat[x][y] == 254 && (contador == 2 || contador ==3))
        mat[x][y] = 6; // es o y vive
}

/*void estadoFuturo(int** mat,int cf, int cc,SDL_Rect fr, SDL_Renderer* render)
{
    int x,y;
    for(x=0; x< cf; x++)
    {
        for(y=0; y<cc; y++)
        {
            if(mat[x][y] == 3 || mat[x][y] == 5)
                mat[x][y] = 32;
            else
            {
                mat[x][y] = 254;
                //pintar(x,y,fr,render);
                SDL_RenderPresent(render);
            }

        }
    }


}*/




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

void manejarClick(SDL_Event e, int** mat, int cf, SDL_Rect fr, int* turno) {
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int mouseX = e.button.x;
        int mouseY = e.button.y;

        int cellWidth = fr.w / cf;
        int cellHeight = fr.h / cf;

        int col = (mouseX - fr.x) / cellWidth;
        int fila = (mouseY - fr.y) / cellHeight;

        if (fila >= 0 && fila < cf && col >= 0 && col < cf) {
            if (mat[fila][col] == 0) {
                mat[fila][col] = (*turno == 0) ? 1 : 2;
                *turno = (*turno + 1) % 2;
            }
        }
    }
}

void jugarMaquina(int** mat, int cf) {
    // Inicializar aleatorio solo una vez
    static int inicializado = 0;
    if (!inicializado) {
        srand(time(NULL));
        inicializado = 1;
    }

    int libres[9][2];
    int cantLibres = 0;

    for (int i = 0; i < cf; i++) {
        for (int j = 0; j < cf; j++) {
            if (mat[i][j] == 0) {
                libres[cantLibres][0] = i;
                libres[cantLibres][1] = j;
                cantLibres++;
            }
        }
    }

    if (cantLibres > 0) {
        int r = rand() % cantLibres;
        int fila = libres[r][0];
        int col = libres[r][1];
        mat[fila][col] = 2; // Jugada de la máquina (O)
    }
}

int verificarGanador(int** mat){
    for (int i = 0; i < 3; i++) {
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

int esEmpate(int** mat) {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (mat[i][j] == 0)
                return 0; // Todavía hay lugares vacíos

    return 1; // No hay lugares vacíos y no ganó nadie
}


int verificar(int ganador, int** mat)
{
    int done = 0;
    if (ganador == 1)
        {
        printf("¡Gano el jugador (X)!\n");
        done = 1;
        }
        else if(ganador == 2)
        {
            printf("¡Gano la maquina (O)!\n");
            done = 1;
        }else if(esEmpate(mat)){
            printf("¡Empate!\n");
            done = 1;
        }
    return done;
}

