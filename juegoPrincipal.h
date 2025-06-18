#ifndef JUEGOPRINCIPAL_H_INCLUDED
#define JUEGOPRINCIPAL_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "./primitivas/listaSimple.h"
#include "./api/apiHeader.h"

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
#define TAM_PILA 3
#define TAM_CADENA_ARCH 100
#define TAM_CADENA 100
#define TAM_MAX_JSON 200
#define MOV_REALIZADO 1
#define MOV_NO_REALIZADO -1
#define FICHA_VACIA ' '
#define ES_MINUS(x) (x>=97 && x<=122)? 1 : 0
#define ES_MAYUS(x) (x>=65 && x<=90)? 1 : 0


typedef void (*PtrFuncion)();
typedef int (*PtrMovInt)();


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
typedef struct{
    PtrFuncion info[MAX_TURNOS];
    int inicio;
    int fin;
    int tam;
}tCola;
typedef struct{
    PtrMovInt info[TAM_PILA];
    size_t tope;
}tPila;
typedef tNodo* tLista;

int jugar(tLista* listaJugadores, tLista* listaPartidas, int );
int ingresarJugadores(tLista *listaJugadores);
void crearListaJugadores(tLista *lista);
int insertarJugadorEnLista(tLista*, char*);
bool validacionDecision(char decision[]);
void menu( char decision[MAX_NOMBRE]);
void convertirAMayusculas(char []);
void imprimirLista(tLista* lista);
int barajarTurnos(tLista* lista);
void intercambiar( void * a, void* b, size_t sizeElem);
bool jugadorEstaListo();
void normalizacionTexto(char* texto);
int crearArchivoConfig(char nombreArch[20]);
int modificarArchivoConfig(char nombreArch[20]);
int leerPartidasArch(char nombreArch[20]);
int comienzaAJugar(void* infoJugador, int cantPartidas, tLista* listaPartidas);
void asignarFicha(int quienEmpieza);
int generoColaTurnos(tCola* colaDeTurnos, int quienEmpieza);
int determinarQuienEmpieza();

void juegaUsuario();
bool esIngresoValido( int fila, int columna);

void colocarFicha( int fila, int columna, char ficha);
char encontrarPosibleGanador();
int calcularPuntaje(char posibleGanador);

void juegaMaquina();
bool puedeGanar(int* fila, int* columna, char ficha);
int intentarGanarMaquina();
int intentarBloquearMaquina();
int ponerRandomMaquina();

void inicializarTablero();
void mostrarTablero();
void crearCola(tCola *cola);
int encolar(tCola *cola, PtrFuncion funcion);
PtrFuncion desencolar(tCola *cola);
void crearPila(tPila *pila);
int estaLlena(tPila* pila);
int estaVacia(tPila*pila);
int pushPila(tPila *pila, PtrMovInt funcion);
void creoYllenoPilaMaquina(tPila *pila);
PtrMovInt popPila(tPila *pila);
void creoYllenoPilaMaquina(tPila *pila);

int obtenerRanking(tLista*, tConfiguracion*);
int obtenerDatosArchivoConfiguracion(char* ruta_arch, tConfiguracion* configuracion);
int compararPuntajeTotal(const void*, const void*);
int compararIgualdadPuntajeTotal(const void* a, const void* b);
void enviarDatosListaAPI(tLista* listaJugadores, tConfiguracion* configuracion, void(*accion)(const void*, const void*));
void leerCadena(char *cadena, char *destino, size_t tam);
void registrarPartida(tLista* listaPartidas, void* jugador, int puntajeObtenido);
void menuSecreto();
void muestraTableroGUI();
#endif // JUEGOPRINCIPAL_H_INCLUDED
