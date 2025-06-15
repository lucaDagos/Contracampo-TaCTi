#ifndef JUEGOPRINCIPAL_H_INCLUDED
#define JUEGOPRINCIPAL_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "listaSimple.h"
#include "apiHeader.h"

#define MAX_NOMBRE 50
#define HAY_ERROR 1
#define EXITO 0

#define TAM_TABLERO 3
#define TAM_CADENA_ARCH 100
#define TAM_CADENA 100

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

typedef tNodo* tLista;
int jugar();
int ingresarJugadores(tLista *listaJugadores);
void crearListaJugadores(tLista *lista);
int insertarJugadorEnLista(tLista *listaJugadores, char* jugador);
bool validacionDecision(char decision[]);
void menu( char decision[MAX_NOMBRE]);
void imprimirLista(tLista* lista);


void iniciarTablero(char tablero[][TAM_TABLERO]);
void mostrarTablero(char tablero[][TAM_TABLERO]);
char verificarGanador(char tablero[][TAM_TABLERO]);
int puedeGanar(char tablero[][TAM_TABLERO], char, int*, int*);
void movIA(char tablero[][TAM_TABLERO], char, int);

int obtenerDatosArchivoConfiguracion(char* ruta_arch, tConfiguracion* configuracion);

//////////////////////////////////////////////////////////////

int obtenerRanking(tLista*, tConfiguracion*);


#endif // JUEGOPRINCIPAL_H_INCLUDED
