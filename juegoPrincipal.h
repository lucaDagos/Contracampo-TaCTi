#ifndef JUEGOPRINCIPAL_H_INCLUDED
#define JUEGOPRINCIPAL_H_INCLUDED

#include <stdio.h>

#include "listaSimple.h"
#include "apiHeader.h"

#define TAM_TABLERO 3
#define TAM_CADENA_ARCH 100
#define TAM_CADENA 100
#define TAM_MAX_JSON 200

typedef struct{
    char urlApi[TAM_CADENA_ARCH];
    char codIdenGrupo[TAM_CADENA];
    int CantPartidas;
} tConfiguracion;

typedef struct{
    char nombre[TAM_CADENA];
    int puntaje;
} tJugador;

typedef struct{
    char jugador[TAM_CADENA];
    int puntajeObtenido;
    char tablero[TAM_TABLERO][TAM_TABLERO];
} tPartida;

void iniciarTablero(char tablero[][TAM_TABLERO]);
void mostrarTablero(char tablero[][TAM_TABLERO]);
char verificarGanador(char tablero[][TAM_TABLERO]);
int puedeGanar(char tablero[][TAM_TABLERO], char, int*, int*);
void movIA(char tablero[][TAM_TABLERO], char, int);

#endif // JUEGOPRINCIPAL_H_INCLUDED
