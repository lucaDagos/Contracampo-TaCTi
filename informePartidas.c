#include "informePartidas.h"
#include "juegoPrincipal.h"

#include <string.h>

void registrarEnInformePuntosJugador(FILE* arch, int PuntosTotales){
    fprintf(arch,"TOTAL DE PUNTOS: %d \n----------------------------------\n\n", PuntosTotales);
    fflush(arch);
}

void registrarMayoresPuntajesEnInforme(FILE* arch, tLista* listaJugadores, int (*cmp)(const void*, const void*)){

    // Se escriben los mayores puntajes en el archivo de texto
    tLista* aux = listaJugadores;

    fprintf(arch,"Puntaje/Puntajes mas altos:\n\n");

    // Si hay varios jugadores con el mismo 'mayor puntaje', los muestra a todos
    while(*listaJugadores && cmp((*listaJugadores)->info, (*aux)->info)){
        fprintf(arch,"Jugador: %s ---> Puntos: %d\n",((tJugador*)(*listaJugadores)->info)->nombre,((tJugador*)(*listaJugadores)->info)->puntaje);
        listaJugadores = &(*listaJugadores)->sig;
    }
}

int registrarPartidasJugadorEnInforme(FILE* arch, tLista* partidasJugadas, int numeroDePartida){

    unsigned j;

    tPartida* auxPartida = (tPartida*) (*partidasJugadas)->info;

    // Se escribe el nombre del jugador en la primer partida
    if(numeroDePartida == 1)
        fprintf(arch,"Jugador: %s\n\n", auxPartida ->jugador);

    fprintf(arch,"Partida numero %d: \n\n",numeroDePartida);
    // Se muestra el tablero
    for(j=0; j<TAM_TABLERO; j++)
        fprintf(arch,"\t%c | %c | %c\n",auxPartida ->tablero[j][0],auxPartida ->tablero[j][1],auxPartida ->tablero[j][2]);
    // Se guardan los puntos obtenidos
    fprintf(arch,"\nPuntos Obtenidos: %d \n\n",auxPartida ->puntajeObtenido);
    // Fuerza la escritura en el archivo, asegurando que los datos no queden en el buffer
    fflush(arch);
    // Se devuelve los puntos obtenidos
    return auxPartida->puntajeObtenido;
}

int crearInformePartidas(tLista* listaJugadores, tLista* partidasJugadas, int cantidadPartidas,
                         int (*cmp)(const void*, const void*)){

    char nombreArch[TAM_CADENA_ARCH];
    FILE* arch;
    int i=1, totalPuntaje=0;
    time_t t = time(NULL);
    struct tm tiempoAct = *localtime(&t);

    // Se crea el nombre del archivo con la fecha y hora
    snprintf(nombreArch, sizeof(nombreArch), "informe-juego_%d-%02d-%02d-%02d-%02d.txt",
             tiempoAct.tm_year + 1900, tiempoAct.tm_mon + 1, tiempoAct.tm_mday, tiempoAct.tm_hour, tiempoAct.tm_min);

    arch = fopen(nombreArch, "wt");
    if(!arch) return HAY_ERROR;

    while(*partidasJugadas){

        // Procesa hasta "cantidadPartidas" registros o hasta que se acaben las partidas.
        while(i<=cantidadPartidas && *partidasJugadas){
            totalPuntaje += registrarPartidasJugadorEnInforme(arch, partidasJugadas, i);
            partidasJugadas = &(*partidasJugadas)->sig;
            i++;
        }

        // Se registran los jugadores con mayores puntajes
        registrarEnInformePuntosJugador(arch, totalPuntaje);
        totalPuntaje=0;
        i=1;

    }

    registrarMayoresPuntajesEnInforme(arch, listaJugadores, cmp);

    fclose(arch);

    return TODO_OK;
}
