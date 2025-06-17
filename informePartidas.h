#ifndef INFORME_H_INCLUDED
#define INFORME_H_INCLUDED
#include <stdio.h>
#include "listaSimple.h"

#define TAM_CADENA_ARCH 100

void registrarEnInformePuntosJugador(FILE* arch, int PuntosTotales);

void registrarMayoresPuntajesEnInforme(FILE* arch, tLista* listaJugadores, int (*cmp)(const void*, const void*));

int registrarPartidasJugadorEnInforme(FILE* arch, tLista* partidasJugadas, int numeroDePartida);

int crearInformePartidas(tLista* listaJugadores, tLista* partidasJugadas, int cantidadPartidas, int (*cmp)(const void*, const void*));

#endif // INFORME_H_INCLUDED
