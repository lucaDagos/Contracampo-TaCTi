#include "juegoPrincipal.h"

void inicializarTablero(char tablero[][TAM_TABLERO]){
    for(int i = 0; i < TAM_TABLERO; i++){
        for(int j = 0; j < TAM_TABLERO; j++){
            tablero[i][j] = ' ';
        }
    }
}

void mostrarTablero(char tablero[][TAM_TABLERO]){

    printf("\n\n\n");

    for(int i = 0; i < TAM_TABLERO; i++){

        for(int fila = 0; fila < 4; fila++){

            printf("           ");

            for(int j = 0; j < TAM_TABLERO; j++){

                if (fila == 2)
                    printf("   %c   ", tablero[i][j]);
                else
                    printf("       ");
                if (j < TAM_TABLERO - 1) printf("|");

            }

            printf("\n");
        }

        if (i < TAM_TABLERO - 1)
            printf("           =======|=======|=======\n");
    }

}

char verificarGanador(char tablero[][TAM_TABLERO]){

    // verificacion horizontal y vertical
    for(int i = 0; i < TAM_TABLERO; i++)
    {
        if (tablero[i][0] != ' ' && tablero[i][0] == tablero[i][1] && tablero[i][1] == tablero[i][2]) return tablero[i][0];
        if (tablero[0][i] != ' ' && tablero[0][i] == tablero[1][i] && tablero[1][i] == tablero[2][i]) return tablero[0][i];
    }

    // verifica diagonales
    if (tablero[0][0] != ' ' && tablero[0][0] == tablero[1][1] && tablero[1][1] == tablero[2][2]) return tablero[0][0];
    if (tablero[0][2] != ' ' && tablero[0][2] == tablero[1][1] && tablero[1][1] == tablero[2][0]) return tablero[0][2];

    return ' ';
}

int puedeGanar(char tablero[][TAM_TABLERO], char jugador, int* fila, int* columna){

    for (int i = 0; i < TAM_TABLERO; i++)
    {
        for (int j = 0; j < TAM_TABLERO; j++)
        {
            if (tablero[i][j] == ' ')
            {
                tablero[i][j] = jugador;

                if (verificarGanador(tablero) == jugador)
                {
                    *fila = i;
                    *columna = j;
                    tablero[i][j] = ' ';
                    return 1;
                }

                tablero[i][j] = ' ';
            }
        }
    }

    return 0;
}

void movIA(char tablero[TAM_TABLERO][TAM_TABLERO], char letraIA, int dificultad){

    int fila, columna;
    char letraJug;

    (letraIA == 'X') ? (letraJug = 'O') : (letraJug = 'X');

    if(dificultad == 1)
    {
        // si la IA puede ganar, lo hace. si no puede ganar, entonces bloquea al jugador.
        if (puedeGanar(tablero, letraIA, &fila, &columna) || puedeGanar(tablero, letraJug, &fila, &columna))
        {
            tablero[fila][columna] = letraIA;
            return;
        }
    }
    else
    {
        // la IA solo verifica si puede ganar.
        if (puedeGanar(tablero, letraIA, &fila, &columna))
        {
            tablero[fila][columna] = letraIA;
            return;
        }
    }
    //movimiento aleatorio si no se encuentra una jugada ganadora o de bloqueo
    do
    {
        fila = rand() % TAM_TABLERO;
        columna = rand() % TAM_TABLERO;
    }
    while (tablero[fila][columna] != ' ');

    tablero[fila][columna] = letraIA;

    return;
}
