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
#define COMIENZA_MAQUINA 0
#define COMIENZA_JUGADOR 1
#define MAX_TURNOS 9
#define TAM_TABLERO 3
#define PUNTAJE_GANO_JUGADOR 3
#define PUNTAJE_GANO_MAQUINA -1
#define PUNTAJE_EMPATE 2
#define TAM_CADENA_ARCH 100
#define TAM_CADENA 100
#define TAM_MAX_JSON 200
#define ES_MINUS(x) (x>=97 && x<=122)? 1 : 0
#define ES_MAYUS(x) (x>=65 && x<=90)? 1 : 0

typedef void (*TurnoFuncion)();

typedef struct{
    char urlApi[TAM_CADENA_ARCH];
    char codIdenGrupo[TAM_CADENA];
    int CantPartidas;
} tConfiguracion;

typedef struct{
    char nombre[MAX_NOMBRE];
    int puntaje;
} tJugador;

typedef struct{
    char jugador[MAX_NOMBRE];
    int puntajeObtenido;
    char tablero[TAM_TABLERO][TAM_TABLERO];
} tPartida;

typedef struct{
    TurnoFuncion info[MAX_TURNOS];
    int inicio;
    int fin;
    int tam;
}tCola;

typedef tNodo* tLista;

int jugar(tLista*, tLista*, char nombreArch[20]);
int ingresarJugadores(tLista *listaJugadores);
void crearListaJugadores(tLista *lista);
int insertarJugadorEnLista(tLista *listaJugadores, char* jugador);
bool validacionDecision(char decision[]);
void menu( char decision[MAX_NOMBRE]);
void imprimirLista(tLista* lista);
int barajarTurnos(tLista* lista);
void intercambiar( void * a, void* b, size_t sizeElem);
bool jugadorEstaListo();
void normalizacionTexto(char* texto);
int crearArchivoConfig(char[30], int);
int leerPartidasArch(char nombreArch[20]);
int comienzaAJugar(tJugador*, char nombreArch[20], tLista*);
void asignarFicha(int quienEmpieza);
int generoColaTurnos(tCola* colaDeTurnos, int quienEmpieza);
int determinarQuienEmpieza();
void juegaUsuario();
void juegaMaquina();
bool esIngresoValido( int fila, int columna);
void colocarFicha( int fila, int columna, char ficha);
char encontrarPosibleGanador();
int calcularPuntaje(char posibleGanador);

void inicializarTablero();
void mostrarTablero();
int puedeGanar(char tablero[][TAM_TABLERO], char, int*, int*);
void movIA(char tablero[][TAM_TABLERO], char, int);

//////////////////FUNCIONES COLA (PODRIA PASARSE A OTRO ARCHIVO COMO SE HACE CON LISTA)
void crearCola(tCola *cola);
int encolar(tCola *cola, TurnoFuncion funcion);
TurnoFuncion desencolar(tCola *cola);
int obtenerDatosArchivoConfiguracion(char* ruta_arch, tConfiguracion* configuracion);

//////////////////////////////////////////////////////////////

int obtenerRanking(tLista*, tConfiguracion*);
int compararPuntajeTotal(const void*, const void*);
int compararIgualdadPuntajeTotal(const void*, const void*);
void enviarDatosListaAPI(tLista*, tConfiguracion*, void(*accion)(const void*, const void*));


#endif // JUEGOPRINCIPAL_H_INCLUDED
