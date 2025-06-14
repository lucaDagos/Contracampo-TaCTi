#include "juegoPrincipal.h"
#include "listaSimple.h"

int jugar(tLista *listaJugadores){
    listaCrear(listaJugadores);
    if(ingresarJugadores(listaJugadores) == ERROR){
        printf("hubo un error en la carga\n");
        return ERROR;
    }
    imprimirLista(listaJugadores);
    return EXITO;
}

int ingresarJugadores(tLista *listaJugadores){
    char jugador[MAX_NOMBRE];
    int errores = 0;
    do{
        printf("Ingrese los nombres de los jugadores(ingrese 'c' para terminar con la carga):\n");
        scanf("%s", jugador);
        if(strcmp(jugador, "c") ==0){
            return EXITO;
        }
        errores = insertarJugadorEnLista(listaJugadores, jugador);
    }while(errores == 0);


    if(errores > 0){
        return ERROR;
    }
    return EXITO;
}

void imprimirLista(tLista *lista){
    if(*lista == NULL){
        printf("lista vacia");
        return;
    }
    //Aca hacemos esto para ahorrarnos desreferencias (*)
    tNodo *aux = *lista;
    int idx = 0;

    while(aux) {
        printf("  [%2d] %s\n", idx, (char*)aux->info);
        aux = aux->sig;
        idx++;
    }
}

//Recordar que este listaJugadores es un puntero al inicio, y que se manda por copia. Dejarlo localmente al final no te jode
int insertarJugadorEnLista(tLista *listaJugadores, char* jugador){
    tNodo *nuevo = (tNodo*)malloc(sizeof(tNodo));
    if(!nuevo){
        printf("Error al asignar memoria para un nodo nuevo");
        return ERROR;
    }

    nuevo->info = malloc(MAX_NOMBRE-1);
    memcpy(nuevo->info, jugador, MAX_NOMBRE - 1);
    nuevo->tamInfo = MAX_NOMBRE - 1;
    nuevo->sig = NULL;

    while(*listaJugadores) {
        listaJugadores = &(*listaJugadores)->sig;
    }
    *listaJugadores = nuevo;
    return EXITO;
}

void crearListaJugadores(tLista *lista){
    *lista = NULL;
}

void menu( char decision[MAX_NOMBRE]){
    printf("[A] Jugar \n[B] Ver ranking equipo \n[C] Salir \n");
    scanf("%s", decision);
    while(validacionDecision(decision) == false){
        printf("ingrese una opcion correcta: \n");
        scanf("%s", decision);
    }
}

bool validacionDecision(char decision[]){
    if(strcmp(decision,"A")==0 || strcmp(decision,"B")==0 || strcmp(decision,"C")==0){
        return true;
    }else
        return false;
}

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

int obtenerDatosArchivoConfiguracion(char* ruta_arch, tConfiguracion* configuracion){

    char cadena[TAM_CADENA_ARCH];

    FILE* arch = fopen(ruta_arch, "rt");
    if(!arch) return ERROR;

    fgets(cadena,TAM_CADENA_ARCH,arch);
    sscanf(cadena,"%[^|]|%[^\n]",configuracion->urlApi,configuracion->codIdenGrupo);

    fgets(cadena,TAM_TABLERO,arch);
    sscanf(cadena,"%d",&configuracion->CantPartidas);

    fclose(arch);

    return EXITO;
}
