#include "juegoPrincipal.h"
#include "listaSimple.h"

//esto es el contexto en una partida para evitar la repeticion en los parametros
char simboloJugador;
char simboloMaquina;
char tablero[N][N];
//REINICIAR TABLERO DESPUES DE CADA PARTIDA
///////////////////////////////////////////////////////////*///////////////////////////////////
//--FUNCIONES PRINCIPALES
/////////////////////////////////////////////////////////////////////////////////////////////*/
int jugar(tLista *listaJugadores, char nombreArch[20]){
    char jugador[MAX_NOMBRE];

    //creo la lista de jujadores y le pido que ingrese el nombre de los que van a jugar
    listaCrear(listaJugadores);
    if(ingresarJugadores(listaJugadores) == ERROR){
        printf("hubo un error en la carga\n");
        return ERROR;
    }

    barajarTurnos(listaJugadores);
    imprimirLista(listaJugadores);
    if(jugadorEstaListo()){
        printf("primer jugador listo\n");
        while(!listaVacia(listaJugadores)){
            sacarPrincipioLista(listaJugadores, jugador,sizeof(jugador));
            comienzaAJugar(jugador,nombreArch);
        }
    }
    else{
         printf("primer jugador NO listo");
    }
    return EXITO;
}
///////////////////////////////////////////////////////////*///////////////////////////////////
//--LOGICA DE JUEGO
/////////////////////////////////////////////////////////////////////////////////////////////*/
int comienzaAJugar(char nombreJugador[MAX_NOMBRE] ,char nombreArch[20]){
    int cantPartidas;
    int puntaje = 0;
    tCola colaDeTurnos;
    cantPartidas = leerPartidasArch(nombreArch);

    while(cantPartidas > 0){
        int primero = determinarQuienEmpieza();
        generoColaTurnos(&colaDeTurnos, primero);
        asignarFicha(primero);
        printf("Partidas restantes de %s:  %d\n", nombreJugador, cantPartidas);
        printf("%s comienza con '%c'\n", primero == COMIENZA_JUGADOR ? "Usuario" : "Maquina",
               primero == COMIENZA_JUGADOR ? simboloJugador : simboloMaquina);


        inicializarTablero();
        mostrarTablero();

        for(int i =0; i < MAX_TURNOS && encontrarPosibleGanador() == ' '; i++){
            TurnoFuncion turno = desencolar(&colaDeTurnos);
            if(turno == NULL){
                return ERROR;
            }
            turno();
            mostrarTablero(tablero);
        }
        puntaje += calcularPuntaje(encontrarPosibleGanador());

        cantPartidas--;
    }
    printf("Jugador: %s           Puntaje: %d\n", nombreJugador, puntaje);
    return puntaje;
}
int calcularPuntaje(char posibleGanador){
    if(posibleGanador == simboloJugador){
        return PUNTAJE_GANO_JUGADOR;
    }
    if(posibleGanador == simboloMaquina){
        return PUNTAJE_GANO_MAQUINA;
    }
    return PUNTAJE_EMPATE;

}
void asignarFicha(int quienEmpieza){
    if(quienEmpieza == COMIENZA_JUGADOR){
        simboloJugador = 'X';
        simboloMaquina = 'O';
    }else{
        simboloJugador = 'O';
        simboloMaquina = 'X';
    }
}
int generoColaTurnos(tCola* colaDeTurnos, int quienEmpieza){
    crearCola(colaDeTurnos);
    for(int i = 0 ; i < MAX_TURNOS; i++){
        int esTurnoUsuario = (i % 2 ==0) ? (quienEmpieza == COMIENZA_JUGADOR): (quienEmpieza == COMIENZA_MAQUINA);
        encolar(colaDeTurnos, esTurnoUsuario ? juegaUsuario : juegaMaquina);
    }
    return EXITO;
}
//si es 0 empieza la maquina, si es 1 el jugador
int determinarQuienEmpieza(){
    srand((unsigned)time(NULL));
    int num = rand() % 2;
    return num;
}
void juegaUsuario(){
    int fila, columna;
    printf("[usuario] ingresa fila y columna (1-%d): \n", N);
    scanf("%d %d", &fila, &columna);
    if(esIngresoValido( fila - 1, columna-1)){
        colocarFicha( fila-1, columna-1,simboloJugador);
    }else{
        printf("Movimiento invalido, usted ingreso :%d%d. Intenta De nuevo...\n", fila, columna);
        juegaUsuario(tablero);
    }
}
void juegaMaquina(){
}
bool esIngresoValido(int fila, int columna){
    if(fila < 0 || fila >= N || columna < 0 || tablero[fila][columna] != ' '){
        return false;
    }
    return true;
}
void colocarFicha( int fila, int columna, char ficha){
    tablero[fila][columna] = ficha;
}
///////////////////////////////////////////////////////////*///////////////////////////////////
//--LOGICA DE TURNOS
/////////////////////////////////////////////////////////////////////////////////////////////*/
int barajarTurnos(tLista* lista){
    int cantJugadores = obtenerTamanioLista(lista);
    if(cantJugadores == 1 ){
        return EXITO;
    }
    tNodo** vectorDeJugadores = malloc(cantJugadores * sizeof(tNodo));
    if(!vectorDeJugadores){
        return ERROR;
    }
    //paso a vector
    tNodo *actual = *lista;
    for(int i = 0; i < cantJugadores; i++){
        vectorDeJugadores[i] = actual;
        actual = actual->sig;
    }
    //random
    srand((unsigned)time (NULL));
    for(int i = cantJugadores - 1; i > 0; i--){
        int j = rand() % ( i + 1 );
        intercambiar(&vectorDeJugadores[i], &vectorDeJugadores[j], sizeof(tNodo*));
    }
    *lista = vectorDeJugadores[0];
    actual = *lista;
    for(int i = 1; i < cantJugadores; i++){
        actual->sig = vectorDeJugadores[i];
        actual = actual->sig;
    }
    actual->sig = NULL;
    free(vectorDeJugadores);
    return EXITO;
}
void intercambiar( void * a, void* b, size_t sizeElem)
{
    void*temp = malloc(sizeElem);
    memcpy(temp, a, sizeElem);
    memcpy(a, b, sizeElem);
    memcpy(b, temp, sizeElem);
    free(temp);
}
///////////////////////////////////////////////////////////*///////////////////////////////////
//--FUNCIONES CARGA DE JUGADORES
/////////////////////////////////////////////////////////////////////////////////////////////*/
int ingresarJugadores(tLista *listaJugadores){
    char jugador[MAX_NOMBRE];
    int errores = 0;
    do{
        printf("Ingrese los nombres de los jugadores(ingrese 'c' para terminar con la carga):\n");
        scanf("%s", jugador);
        if(strcmp(jugador, "c") == 0){
            return EXITO;
        }
        errores = insertarJugadorEnLista(listaJugadores, jugador);
    }while(errores == 0);


    if(errores > 0){
        return ERROR;
    }
    return EXITO;
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
///////////////////////////////////////////////////////////*///////////////////////////////////
//--LOGICA ARCHIVO CONFIGURACION
/////////////////////////////////////////////////////////////////////////////////////////////*/
int crearArchivoConfig(char nombreArch[20],int cantPartidas){
    FILE *arch = fopen(nombreArch, "w");
    if(!arch){
        printf("error al crear el archivo de configuracion\n");
        return ERROR;
    }
    fprintf(arch, "numero_de_partidas=%d\n", cantPartidas);
    fclose(arch);
    return EXITO;
}
int leerPartidasArch(char nombreArch[20]){
    FILE *arch = fopen(nombreArch, "r");
    char linea[100];
    int partidas = -1;
    if(!arch){
        printf("error al crear el archivo de configuracion\n");
        return ERROR;
    }
    fgets(linea, sizeof(linea), arch);
    sscanf(linea, "numero_de_partidas=%d", &partidas);
    fclose(arch);
    if(partidas <= 0){
        printf("valor invalido de numero de partidas: %d\n", partidas);
        return ERROR;
    }
    return partidas;
}

///////////////////////////////////////////////////////////*///////////////////////////////////
//--FUNCIONES AUXILIARES
/////////////////////////////////////////////////////////////////////////////////////////////*/
bool jugadorEstaListo(){
    char respuesta;
    while (1) {
        printf("estas listo para comenzar a jugar? (Y/N): ");
        scanf(" %c", &respuesta);
        normalizacionTexto(&respuesta);
        if (respuesta == 'Y') {
            return true;
        }
        else if (respuesta == 'N') {
            return false;
        }
        printf("Respuesta no valida, ingresa Y o N\n");
    }
}
void normalizacionTexto(char* texto){

    if (ES_MINUS(*texto)){
        *texto = *texto + ('A' - 'a');
    }
    texto++;
    while (*texto != '\0'){
        if(ES_MAYUS(*texto)){
            *texto = *texto - ('A' - 'a');
        }
        if((*(texto-1)==' ') && (ES_MINUS(*texto))){
            *texto = *texto + ('A' - 'a');
        }
        texto++;
    }
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
///////////////////////////////////COLA FUNCIONES
void crearCola(tCola *cola){
    cola->inicio = 0;
    cola->fin = -1;
    cola->tam = 0;
}
int encolar(tCola *cola, TurnoFuncion funcion)
{
    if(cola->tam ==  MAX_TURNOS){
        return 0;
    }
    cola->fin = (cola->fin + 1) % MAX_TURNOS;
    cola->info[cola->fin] = funcion;
    cola->tam++;
    return EXITO;
}
TurnoFuncion desencolar(tCola *cola){
    if(cola->tam == 0){
        return NULL;
    }
    TurnoFuncion turno = cola->info[cola->inicio];
    cola->inicio = (cola->inicio + 1) % MAX_TURNOS;
    cola->tam--;
    return turno;
}

///////////////////////////////////////////////////////////////////////
void inicializarTablero(){
    for(int i = 0; i < TAM_TABLERO; i++){
        for(int j = 0; j < TAM_TABLERO; j++){
            tablero[i][j] = ' ';
        }
    }
}
void mostrarTablero(){

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

char encontrarPosibleGanador(){

    // verificacion horizontal y vertical
    for(int i = 0; i < N; i++)
    {
        if (tablero[i][0] != ' ' && tablero[i][0] == tablero[i][1] && tablero[i][1] == tablero[i][2]) return tablero[i][0];
        if (tablero[0][i] != ' ' && tablero[0][i] == tablero[1][i] && tablero[1][i] == tablero[2][i]) return tablero[0][i];
    }

    if (tablero[0][0] != ' ' && tablero[0][0] == tablero[1][1] && tablero[1][1] == tablero[2][2]) return tablero[0][0];
    if (tablero[0][2] != ' ' && tablero[0][2] == tablero[1][1] && tablero[1][1] == tablero[2][0]) return tablero[0][2];

    return ' ';
}
/*
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
*/
