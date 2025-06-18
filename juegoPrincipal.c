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

    listaCrear(listaJugadores);
    if(ingresarJugadores(listaJugadores) == HAY_ERROR){
        printf("hubo un error en la carga\n");
        return HAY_ERROR;
    }
    barajarTurnos(listaJugadores);
    imprimirLista(listaJugadores);
    if(jugadorEstaListo()){
        printf("primer jugador listo\n");
        while(*listaJugadores){
            //sacarPrincipioLista(listaJugadores, jugador,sizeof(jugador));
            comienzaAJugar((*listaJugadores)->info, cantPart, listaPartidas);
            listaJugadores = &(*listaJugadores)->sig;
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

void registrarPartida(tLista* listaPartidas, void* jugador, int puntajeObtenido){

    tPartida partida;
    tJugador* auxJugador = (tJugador*) jugador;
    int i,j;

    for(i=0; i<TAM_TABLERO; i++){
        for(j=0; j<TAM_TABLERO; j++){
            partida.tablero[i][j] = tablero[i][j];
        }
    }
    partida.puntajeObtenido = puntajeObtenido;
    strcpy(partida.jugador, auxJugador->nombre);
    listaInsertarAlFinal(listaPartidas, &partida, sizeof(tPartida));
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
    scanf("%d%d",&fila,&columna);
    if(esIngresoValido( fila - 1, columna-1)){
        colocarFicha( fila-1, columna-1,simboloJugador);
    }else{
        printf("Movimiento invalido, usted ingreso :%d%d. Intenta De nuevo...\n", fila, columna);
        juegaUsuario();
    }
}
void juegaMaquina(){

    tPila pilaDeMovimientos;
    creoYllenoPilaMaquina(&pilaDeMovimientos);
    while(!estaVacia(&pilaDeMovimientos)){
        PtrMovInt movimiento = popPila(&pilaDeMovimientos);
        if(movimiento() == MOV_REALIZADO){
            return;
        }
    }
}
void creoYllenoPilaMaquina(tPila *pila){
    crearPila(pila);
    pushPila(pila, ponerRandomMaquina);
    pushPila(pila, intentarBloquearMaquina);
    pushPila(pila, intentarGanarMaquina);
}

int intentarGanarMaquina(){
    int fila, columna;
     if(puedeGanar(&fila, &columna, simboloMaquina)){
       colocarFicha(fila, columna, simboloMaquina);
        return MOV_REALIZADO;
     }
     return MOV_NO_REALIZADO;
}
int intentarBloquearMaquina() {
    int fila, columna;
     if(puedeGanar(&fila, &columna, simboloJugador)){
        colocarFicha(fila, columna, simboloMaquina);
        return MOV_REALIZADO;
     }
     return MOV_NO_REALIZADO;
}
int ponerRandomMaquina() {
    int fila, columna;
    do
    {
        fila = rand() % TAM_TABLERO;
        columna = rand() % TAM_TABLERO;
    }
    while (tablero[fila][columna] != ' ');

    colocarFicha(fila, columna, simboloMaquina);

    return MOV_REALIZADO;
}
bool puedeGanar(int* fila, int* columna, char ficha){

    for (int i = 0; i < TAM_TABLERO; i++)
    {
        for (int j = 0; j < TAM_TABLERO; j++)
        {
            if (tablero[i][j] == ' ')
            {
               colocarFicha(i,j,ficha);

                if (encontrarPosibleGanador() == ficha)
                {
                    *fila = i;
                    *columna = j;
                    colocarFicha(i,j,FICHA_VACIA);
                    return true;
                }

                tablero[i][j] = FICHA_VACIA;
            }
        }
    }
    return false;
}

bool esIngresoValido(int fila, int columna){
    if(fila < 0 || fila >= TAM_TABLERO || columna < 0 || tablero[fila][columna] != FICHA_VACIA){
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
    tNodo *actual = *lista;
    for(int i = 0; i < cantJugadores; i++){
        vectorDeJugadores[i] = actual;
        actual = actual->sig;
    }
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
    char esjugador_invalido=0;
    int errores = 0;
    do{
        printf("Ingrese los nombres de los jugadores(ingrese 'c' para terminar con la carga):\n");
        scanf("%s", jugador);
        while(esjugador_invalido==0 && strcmp(jugador, "c") == 0)
        {
            printf("Debe ingresar al menos un jugador para comenzar el juego):\n");
            scanf("%s", jugador);
        }
        if(strcmp(jugador, "c") == 0){
            return EXITO;
        }
        errores = insertarJugadorEnLista(listaJugadores, jugador);
        esjugador_invalido=1;
    }while(errores == 0);

    if(errores > 0){
        return HAY_ERROR;
    }
    return EXITO;
}

int insertarJugadorEnLista(tLista *listaJugadores, char* nom_jug){
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
    FILE *arch = fopen(nombreArch, "w");
    if(!arch){
        printf("Error al crear el archivo de configuracion\n");
        return ERROR;
    }
    tConfiguracion config;

    strcpy(config.urlApi, URL_API);
    strcpy(config.codIdenGrupo, COD_GRUPO);
    config.CantPartidas = CANT_PARTIDAS_CONFIG;

    fprintf(arch, "%s|", config.urlApi);
    fprintf(arch, "%s\n", config.codIdenGrupo);
    fprintf(arch, "%d", config.CantPartidas);
    fclose(arch);
    return EXITO;
}
int modificarArchivoConfig(char nombreArch[20]){
    FILE *arch = fopen(nombreArch, "r");
    if(!arch){
        printf("error al crear el archivo de configuracion\n");
        return ERROR;
    }
    tConfiguracion config;
    fscanf(arch, "%[^|]|", config.urlApi);
    fscanf(arch, "%[^\n]\n", config.codIdenGrupo);
    fscanf(arch, "%d", &config.CantPartidas);
    fclose(arch);
    int opcion;
    do {
        menuSecreto();
        scanf("%d", &opcion);
        switch(opcion){
            case 1:
                leerCadena("Ingrese nueva URL: ", config.urlApi, sizeof(config.urlApi));
                break;
            case 2:
                leerCadena("Ingrese nuevo código de identificación: ", config.codIdenGrupo, sizeof(config.codIdenGrupo));
                break;
            case 3:
                printf("Ingrese nueva cantidad de partidas: ");
                scanf("%d", &config.CantPartidas);
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
    fprintf(arch, "%s|", config.urlApi);
    fprintf(arch, "%s\n", config.codIdenGrupo);
    fprintf(arch, "%d", config.CantPartidas);
    fclose(arch);

    }
    return EXITO;
}
void menuSecreto(){
   printf("\n--- Modificar Configuracion ---\n");
   printf("1. URL de la API\n");
   printf("2. Codigo de identificacion\n");
   printf("3. Cantidad de partidas\n");
   printf("0. Guardar y salir\n");
   printf("Opcion: ");
}
void leerCadena(char *cadena, char *destino, size_t tam) {
    char aux[256];
    printf("%s", cadena);
    if (fgets(aux, sizeof(aux), stdin)) {
        aux[strcspn(aux, "\n")] = '\0';
        strncpy(destino, aux, tam - 1);
        destino[tam - 1] = '\0';
    }
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
void convertirAMayusculas(char cadena[]){
    int i;
    for (i = 0; i < strlen(cadena); i++) {
        cadena[i] = toupper(cadena[i]);
    }
}
void menu( char decision[MAX_NOMBRE]){
    printf("\n[A] Jugar \n[B] Ver ranking equipo \n[C] Salir \n");
    scanf("%s", decision);
    convertirAMayusculas(decision);
    while(validacionDecision(decision) == false){
        printf("ingrese una opcion correcta: \n");
        scanf("%s", decision);
        convertirAMayusculas(decision);
    }
}

bool validacionDecision(char decision[]){
    if(strcmp(decision,"A")==0 || strcmp(decision,"B")==0 || strcmp(decision,"C")==0 || strcmp(decision,"MODIFICARARCHIVO")==0){
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
int encolar(tCola *cola, PtrFuncion funcion)
{
    if(cola->tam ==  MAX_TURNOS){
        return 0;
    }
    cola->fin = (cola->fin + 1) % MAX_TURNOS;
    cola->info[cola->fin] = funcion;
    cola->tam++;
    return EXITO;
}
PtrFuncion desencolar(tCola *cola){
    if(cola->tam == 0){
        return NULL;
    }
    PtrFuncion turno = cola->info[cola->inicio];
    cola->inicio = (cola->inicio + 1) % MAX_TURNOS;
    cola->tam--;
    return turno;
}
///////////////////////////////////PILA FUNCIONES
void crearPila(tPila *pila){
    pila->tope = -1;
}
int estaLlena(tPila* pila){
    return pila->tope == TAM_PILA -1;
}
int estaVacia(tPila* pila){
    return pila->tope== -1;
}
int pushPila(tPila *pila, PtrMovInt funcion){
    if(estaLlena(pila)){
        return ERROR;
    }
    pila->info[++pila->tope] = funcion;
    return EXITO;
}
PtrMovInt popPila(tPila *pila){
    if(estaVacia(pila)){
        return NULL;
    }
    return pila->info[pila->tope--];
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
        if (tablero[i][0] != FICHA_VACIA && tablero[i][0] == tablero[i][1] && tablero[i][1] == tablero[i][2]){
            return tablero[i][0];
        }
        if (tablero[0][i] != FICHA_VACIA && tablero[0][i] == tablero[1][i] && tablero[1][i] == tablero[2][i]){
            return tablero[0][i];
        }
    }

    if (tablero[0][0] != FICHA_VACIA && tablero[0][0] == tablero[1][1] && tablero[1][1] == tablero[2][2]){
        return tablero[0][0];
    }
    if (tablero[0][2] != FICHA_VACIA && tablero[0][2] == tablero[1][1] && tablero[1][1] == tablero[2][0]){
        return tablero[0][2];
    }

    return FICHA_VACIA;
}


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
