//
// Created by Julieta Moreno on 17/06/2025.
//
#include "juegoPrincipal.h"
#include "listaSimple.h"
#include "apiHeader.h"


tLista listaJugadores;
char* nombreArchivo = "config.txt";

void tt_init_players(void){
    listaCrear(&listaJugadores);
}

int tt_add_player(char *name){
    if (insertarJugadorEnLista(&listaJugadores, name) == HAY_ERROR) {
        return HAY_ERROR;
    }
    return EXITO;
}

int tt_get_num_players(void){
    return obtenerTamanioLista(&listaJugadores);
}

char* tt_select_random_player(void){
    static char jugador[MAX_NOMBRE];
    barajarTurnos(&listaJugadores);
    if(!listaVacia(listaJugadores)){
        sacarPrincipioLista(&listaJugadores, jugador, sizeof(jugador));
        return jugador;
    } else{
        return NULL;
    }
}

int  tt_get_num_games(){
    char nombreArch[TAM_CADENA_ARCH] = "config.txt";
    return leerPartidasArch(nombreArch);
}

void tt_game_init(char *name){
    comienzaAJugar(name, nombreArchivo);
}


