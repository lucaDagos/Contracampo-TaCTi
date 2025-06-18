#ifndef LISTASIMPLE_H_INCLUDED
#define LISTASIMPLE_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#define TODO_OK 1
#define LIMITE_ALCANZADO 3
#define ERROR_MEM 2
#define ERROR_ARCH 3
#define ERROR_LISTA 0
#define MIN(X,Y) (X) > (Y) ? (Y) : (X)

typedef struct sNodo{
    void* info;
    unsigned tamInfo;
    struct sNodo *sig;
}tNodo;

typedef tNodo* tLista;

void listaCrear(tLista* l);
int listaInsertarAlInicio(tLista* l,void* elem, unsigned tamElem);
int listaInsertarAlFinal(tLista* l,void* elem, unsigned tamElem);
int listaInsertarEnPosAleatoria(tLista* l, int limite, void* elem, unsigned tamElem, int (*cmp)(const void* a, const void* b));
int insertarOrdenadoLimitado(tLista* pl, int limite, const void* elem, unsigned tamElem, int (*cmp)(const void*, const void*));
int ordenarLista(tLista* l, int (*cmp)(const void*, const void*));
void listaVaciar(tLista* l);
bool listaVacia(tLista* l);
void listaFuncionMap(tLista* l, void (*accion)(const void*,const void*));
int obtenerTamanioLista(tLista *lista);
int sacarPrincipioLista(tLista *lista, void *info, size_t tamInfo);

#endif // LISTASIMPLE_H_INCLUDED
