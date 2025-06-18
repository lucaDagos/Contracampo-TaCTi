#include "juegoPrincipal.h"
#include "listaSimple.h"

//esto es el contexto en una partida para evitar la repeticion en los parametros
char simboloJugador;
char simboloMaquina;
char tablero[TAM_TABLERO][TAM_TABLERO];

//REINICIAR TABLERO DESPUES DE CADA PARTIDA
///////////////////////////////////////////////////////////*///////////////////////////////////
//--FUNCIONES PRINCIPALES
/////////////////////////////////////////////////////////////////////////////////////////////*/

int jugar(tLista* listaJugadores, tLista* listaPartidas, int cantPart){

    tJugador jugador;

    //creo la lista de jujadores y le pido que ingrese el nombre de los que van a jugar
    listaCrear(listaJugadores);
    if(ingresarJugadores(listaJugadores) == HAY_ERROR){
        printf("hubo un error en la carga\n");
        return HAY_ERROR;
    }

    barajarTurnos(listaJugadores);
    imprimirLista(listaJugadores);
    if(jugadorEstaListo()){
        printf("primer jugador listo\n");
        while(!listaVacia(listaJugadores)){
            sacarPrincipioLista(listaJugadores, &jugador, sizeof(tJugador));
            comienzaAJugar(&jugador, cantPart, listaPartidas);
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

<<<<<<< Updated upstream
=======
int comienzaAJugar(void* infoJugador, int cantPartidas, tLista* listaPartidas){
    tJugador* jugador = (tJugador*)infoJugador;
    int puntajeParcial = 0;
    tCola colaDeTurnos;

    while(cantPartidas > 0){
        int primero = determinarQuienEmpieza();
        generoColaTurnos(&colaDeTurnos, primero);
        asignarFicha(primero);
        printf("Partidas restantes de %s:  %d\n", jugador->nombre, cantPartidas);
        printf("%s comienza con '%c'\n", primero == COMIENZA_JUGADOR ? "Usuario" : "Maquina",
               primero == COMIENZA_JUGADOR ? simboloJugador : simboloMaquina);
        inicializarTablero();
        mostrarTablero();
        for(int i =0; i < MAX_TURNOS && encontrarPosibleGanador() == ' '; i++){
            PtrFuncion turno = desencolar(&colaDeTurnos);
            if(turno == NULL){
                return ERROR;
            }
            turno();
            mostrarTablero();
        }
        puntajeParcial = calcularPuntaje(encontrarPosibleGanador());
        jugador->puntaje += puntajeParcial;
        registrarPartida(listaPartidas, jugador, puntajeParcial);
        cantPartidas--;
    }
    printf("Jugador: %s           Puntaje: %d\n", jugador->nombre, jugador->puntaje);

    return jugador->puntaje;
}

>>>>>>> Stashed changes
void registrarPartida(tLista* listaPartidas, void* jugador, int puntajeObtenido){

    tPartida partida;
    tJugador* auxJugador = (tJugador*) jugador;
    int i,j;

    for(i=0; i<TAM_TABLERO; i++){
        for(j=0; j<TAM_TABLERO; j++){
            partida.tablero[i][j] = tablero[i][j];
        }
    }

    //Cargamos el puntaje de la partida
    partida.puntajeObtenido = puntajeObtenido;

    //cargamos el nombre del jugador
    strcpy(partida.jugador, auxJugador->nombre);

    //insertamos toda la información
    listaInsertarAlFinal(listaPartidas, &partida, sizeof(tPartida));

}

int comienzaAJugar(tJugador* jugador, int cantPart, tLista* listaPartidas){

    int cantPartidas;
    int puntParcial;
    tCola colaDeTurnos;
    cantPartidas = cantPart;

    while(cantPartidas > 0){

        int primero = determinarQuienEmpieza();
        generoColaTurnos(&colaDeTurnos, primero);
        asignarFicha(primero);
        printf("Partidas restantes de %s:  %d\n", jugador->nombre, cantPartidas);
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
            mostrarTablero();
        }

        puntParcial = calcularPuntaje(encontrarPosibleGanador());

        jugador->puntaje += puntParcial;

        registrarPartida(listaPartidas, jugador, puntParcial);

        cantPartidas--;
    }

    printf("Jugador: %s           Puntaje: %d\n", jugador->nombre, jugador->puntaje);

    return jugador->puntaje;
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
    printf("[usuario] ingresa fila y columna (1-%d): \n", TAM_TABLERO);
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
    if(fila < 0 || fila >= TAM_TABLERO || columna < 0 || tablero[fila][columna] != ' '){
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

void intercambiar( void * a, void* b, size_t sizeElem){
    void*temp = malloc(sizeElem);
    memcpy(temp, a, sizeElem);
    memcpy(a, b, sizeElem);
    memcpy(b, temp, sizeElem);
    free(temp);
}

///////////////////////////////////////////////////////////*///////////////////////////////////
//--FUNCIONES CARGA DE JUGADORES
/////////////////////////////////////////////////////////////////////////////////////////////*/

int ingresarJugadores(tLista* listaJugadores){

    char jugador[MAX_NOMBRE];
    int errores = 0;

    do{
        printf("Ingrese los nombres de los jugadores (ingrese 'c' para terminar con la carga):\n");
        scanf("%s", jugador);
        fflush(stdin);
        if(strcmp(jugador, "c") == 0){
            return EXITO;
        }
        errores = insertarJugadorEnLista(listaJugadores, jugador);
    }while(errores == 0);

    if(errores > 0){
        return HAY_ERROR;
    }
    return EXITO;
}

<<<<<<< Updated upstream
//Recordar que este listaJugadores es un puntero al inicio, y que se manda por copia. Dejarlo localmente al final no te jode
int insertarJugadorEnLista(tLista* listaJugadores, char* nom_jug){

=======
int insertarJugadorEnLista(tLista *listaJugadores, char* nom_jug){
>>>>>>> Stashed changes
    tJugador jugador;
    strcpy(jugador.nombre, nom_jug);
    jugador.puntaje = 0;

    tNodo* nuevo = (tNodo*) malloc(sizeof(tNodo));
    if(!nuevo){
        printf("Error al asignar memoria para un nodo nuevo");
        return HAY_ERROR;
    }

    nuevo->info = malloc(sizeof(tJugador));
    if(!nuevo->info){
        printf("Error al asignar memoria para la informacion del jugador");
        return HAY_ERROR;
    }

    memcpy(nuevo->info, &jugador, sizeof(tJugador));
    nuevo->tamInfo = sizeof(tJugador);
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

int crearArchivoConfig(char nombreArch[20]){

    FILE* arch = fopen(nombreArch, "w");
    if(!arch){
        printf("Error al crear el archivo de configuración\n");
        return HAY_ERROR;
    }

    tConfiguracion config;

    strcpy(config.urlApi, "https://algoritmos-api.azurewebsites.net/api/TaCTi");
    strcpy(config.codIdenGrupo, "estructura");
    config.CantPartidas = 3;

    fprintf(arch, "%s|", config.urlApi);
    fprintf(arch, "%s\n", config.codIdenGrupo);
    fprintf(arch, "%d", config.CantPartidas);

    fclose(arch);

    return EXITO;
}

int modificarArchivoConfig(char nombreArch[20]){

    FILE* arch = fopen(nombreArch, "r");
    if(!arch){
        printf("Error al abrir el archivo de configuracion\n");
        return HAY_ERROR;
    }

    tConfiguracion config;

    // Leer datos actuales
    fscanf(arch, "%[^|]|", config.urlApi);
    fscanf(arch, "%[^\n]\n", config.codIdenGrupo);
    fscanf(arch, "%d", &config.CantPartidas);

    fclose(arch);

    int opcion;
    char aux[TAM_CADENA];

    do {
        printf("\n--- Modificar Configuracion ---\n");
        printf("1. URL de la API\n");
        printf("2. Codigo de identificacion\n");
        printf("3. Cantidad de partidas\n");
        printf("0. Guardar y salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);
        fflush(stdin);

        switch(opcion){
            case 1:
                printf("Ingrese nueva URL: ");
                fflush(stdin);
                fgets(aux, sizeof(aux), stdin);
                aux[strcspn(aux, "\n")] = 0;
                strcpy(config.urlApi, aux);
                break;
            case 2:
                printf("Ingrese nuevo codigo de identificacion: ");
                fflush(stdin);
                fgets(aux, sizeof(aux), stdin);
                aux[strcspn(aux, "\n")] = 0;
                strcpy(config.codIdenGrupo, aux);
                break;
            case 3:
                printf("Ingrese nueva cantidad de partidas: ");
                scanf("%d", &config.CantPartidas);
                fflush(stdin);
                break;
            case 0:
                printf("Guardando cambios...\n");
                break;
            default:
                printf("Opción invalida\n");
        }

    } while(opcion != 0);

    arch = fopen(nombreArch, "w");
    if(!arch){
        printf("Error al reescribir el archivo de configuracion\n");
        return HAY_ERROR;
    }

    fprintf(arch, "%s|", config.urlApi);
    fprintf(arch, "%s\n", config.codIdenGrupo);
    fprintf(arch, "%d", config.CantPartidas);

    fclose(arch);

    return EXITO;
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

void convertirAMayusculas(char str[]){
    int i;
    for (i = 0; i < strlen(str); i++) {
        str[i] = toupper(str[i]);
    }
}

void menu(char decision[MAX_NOMBRE]){
    printf("[A] Jugar \n[B] Ver ranking equipo \n[C] Salir \n");
    scanf("%s", decision);
    convertirAMayusculas(decision);
    while(validacionDecision(decision) == false){
        printf("ingrese una opcion correcta: \n");
        scanf("%s", decision);
        convertirAMayusculas(decision);
    }
}

bool validacionDecision(char decision[]){
    if( strcmp(decision,"A")==0 || strcmp(decision,"B")==0 || strcmp(decision,"C")==0 || strcmp(decision,"MODIFICARARCHIVO")==0 ){
        return true;
    }else
        return false;
}

int compararPuntajeTotal(const void* a, const void* b){

    const tJugador* jugadorA = (const tJugador*)a;
    const tJugador* jugadorB = (const tJugador*)b;

    if(jugadorA->puntaje < jugadorB->puntaje) return 1;
    if(jugadorA->puntaje > jugadorB->puntaje) return -1;

    return 0;
}

int compararIgualdadPuntajeTotal(const void* a, const void* b){

    const tJugador* jugadorA = (const tJugador*)a;
    const tJugador* jugadorB = (const tJugador*)b;

    return jugadorA->puntaje == jugadorB->puntaje;
}

///////////////////////////////////COLA FUNCIONES

void crearCola(tCola *cola){
    cola->inicio = 0;
    cola->fin = -1;
    cola->tam = 0;
}

int encolar(tCola *cola, TurnoFuncion funcion){
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
    for(int i = 0; i < TAM_TABLERO; i++)
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

int obtenerDatosArchivoConfiguracion(char* ruta_arch, tConfiguracion* configuracion){

    char cadena[TAM_CADENA_ARCH];

    FILE* arch = fopen(ruta_arch, "rt");
    if(!arch) return HAY_ERROR;

    fgets(cadena,TAM_CADENA_ARCH,arch);
    sscanf(cadena,"%[^|]|%[^\n]",configuracion->urlApi, configuracion->codIdenGrupo);

    fgets(cadena,TAM_TABLERO,arch);
    sscanf(cadena,"%d",&configuracion->CantPartidas);

    fclose(arch);

    return EXITO;
}

// Crea una lista (limitada) para el ranking
int obtenerRanking(tLista* lista, tConfiguracion* configuracion){

    CURLcode res;
    tJugadorAPI jugador;
    tRespuesta resAPI = {NULL, 0};
    char pathGet[TAM_CADENA_ARCH];

    snprintf(pathGet, sizeof(pathGet), "%s/%s", configuracion->urlApi, configuracion->codIdenGrupo);

    res = peticionGET(&resAPI, pathGet);
    if (res != CURLE_OK){
        printf("Error en la solicitud a la API.\n");
        return HAY_ERROR;
    }
    else{
        while(parsearJugadores(&resAPI, &jugador))
            insertarOrdenadoLimitado(lista, LIMITE_RANKING, &jugador, sizeof(tJugadorAPI), compararJugAPI);
    }
    free(resAPI.info);
    return TODO_OK;
}

void enviarDatosListaAPI(tLista* listaJugadores, tConfiguracion* configuracion, void(*accion)(const void*, const void*)){

    while(*listaJugadores){
        accion((*listaJugadores)->info, configuracion);
        listaJugadores=&(*listaJugadores)->sig;
    }

}










