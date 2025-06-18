#include "listaSimple.h"

void listaCrear(tLista* l){
    *l = NULL;
}

int listaInsertarAlInicio(tLista* l,void* elem, unsigned tamElem){

    tNodo* nuevo;

    nuevo= malloc(sizeof(tNodo));

    if(!nuevo)
        return ERROR_MEM;

    nuevo->info = malloc(tamElem);

    if(!nuevo->info)
    {
        free(nuevo);
        return ERROR_MEM;
    }

    nuevo->tamInfo=tamElem;
    memcpy(nuevo->info,elem,tamElem);

    nuevo->sig=*l;
    *l=nuevo;

    return TODO_OK;
}

int listaInsertarAlFinal(tLista* l,void* elem,unsigned tamElem){

    tNodo* nuevo= malloc(sizeof(tNodo));

    if(!nuevo)
        return ERROR_LISTA;

    nuevo->info= malloc(tamElem);

    if(!nuevo->info)
    {
        free(nuevo);
        return ERROR_LISTA;
    }

    memcpy(nuevo->info, elem, tamElem);
    nuevo->tamInfo = tamElem;

    if(!*l)
    {
        nuevo->sig=*l;
        *l=nuevo;
        return TODO_OK;
    }

    while((*l)->sig)
    {
        l =&(*l)->sig;
    }
    nuevo->sig = (*l)->sig;
    (*l)->sig = nuevo;

    return TODO_OK;
}

int listaInsertarEnPosAleatoria(tLista* l, int limite, void* elem, unsigned tamElem, int (*cmp)(const void* a, const void* b)){

    tNodo* nuevo;
    tLista* lb = l;
    int n, i=0;

    while(*lb)
    {
        if(!cmp((*lb)->info, elem)){
            return ERROR_LISTA;
        }

        lb = &(*lb)->sig;
    }

    nuevo= malloc(sizeof(tNodo));
    if(!nuevo)
        return ERROR_MEM;

    nuevo->info = malloc(tamElem);
    if(!nuevo->info){
        free(nuevo);
        return ERROR_MEM;
    }

    nuevo->tamInfo=tamElem;
    memcpy(nuevo->info,elem,tamElem);

    if(*l)
    {
        n= rand()%(limite+1);

        while(i<n && (*l)->sig)
        {
            l = &(*l)->sig;
            i++;
        }
    }

    nuevo->sig= *l;
    *l = nuevo;

    return TODO_OK;
}

int insertarOrdenadoLimitado(tLista* pl, int limite, const void* elem, unsigned tamElem, int (*cmp)(const void*, const void*)){

    tNodo* nuevo;
    tNodo* elim;
    int n = 0, resultado = 0;

    while( *pl && n < limite && (resultado = cmp((*pl)->info, elem) > 0)){
        pl = &(*pl)->sig;
        n++;
    }

    if( n == limite && resultado > 0 ){
        return LIMITE_ALCANZADO;
    }

    nuevo = (tNodo*) malloc(sizeof(tNodo));
    if(!nuevo)
        return ERROR_MEM;

    nuevo->info = malloc(tamElem);

    if(!nuevo->info){
        free(nuevo);
        return ERROR_MEM;
    }

    memcpy(nuevo->info, elem, tamElem);
    nuevo->tamInfo = tamElem;
    nuevo->sig = *pl;
    *pl = nuevo;

    if((*pl)->sig && n < limite){

        while((*pl)->sig && n < limite){
            pl = &(*pl)->sig;
            n++;
        }

        //Elimino el ultimo
        if(n == limite){
            elim = *pl;
            *pl = NULL;
            free(elim->info);
            free(elim);
        }
    }

    return TODO_OK;
}

int ordenarLista(tLista* l, int (*cmp)(const void*,const void*)){

    tLista *pri = l;

    if (*l == NULL)
        return 0;

    while ((*l)->sig)
    {
        if (cmp((*l)->info, (*l)->sig->info) > 0)
        {
            tLista *q = pri;

            tNodo *auxNodo = (*l)->sig;

            (*l)->sig = auxNodo->sig;

            while (cmp((*q)->info, auxNodo->info) < 0)
                q = &(*q)->sig;

            auxNodo->sig = *q;
            *q = auxNodo;
        }
        else
            l = &(*l)->sig;
    }

    return 1;
}

void listaVaciar(tLista* l){
    tNodo *elim;
    while(*l)
    {
        elim = *l;
        *l = elim->sig;
        free(elim->info);
        free(elim);
    }
}

bool listaVacia(tLista* l){
    return(*l == NULL);
}

void listaFuncionMap(tLista* l, void (*accion)(const void*,const void*)){
    int i=0;
    while(*l)
    {
        i++;
        accion((*l)->info,&i);
        l=&(*l)->sig;
    }
}

int sacarPrincipioLista(tLista* lista, void* info, size_t tamInfo){

    tNodo* aux = *lista;
    if(listaVacia(lista)){
        return ERROR_LISTA;
    }

    *lista = aux->sig;
    memcpy(info, aux->info, MIN(tamInfo,aux->tamInfo));

    free(aux->info);
    free(aux);

    return TODO_OK;
}

int obtenerTamanioLista(tLista *lista) {
    int contador = 0;
    tNodo *actual = *lista; // desreferenci�s el puntero para obtener el nodo inicial

    while (actual != NULL) {
        contador++;
        actual = actual->sig;
    }

    return contador;
}
