#ifndef APIHEADER_H_INCLUDED
#define APIHEADER_H_INCLUDED

#include "listaSimple.h"
#include "juegoPrincipal.h"

#include <stdio.h>
#include <curl/curl.h>
#include <string.h>

#define TAM_CAD 70

#define TAM_MAX_JSON 200
#define LIMITE_RANKING 5
<<<<<<< Updated upstream
=======
#define URL_API  "https://algoritmos-api.azurewebsites.net/api/TaCTi"
#define COD_GRUPO "estructura"
#define CANT_PARTIDAS_CONFIG 1
>>>>>>> Stashed changes

typedef struct {
    char nombre[TAM_CAD];
    int puntaje;
    char fyh[TAM_CAD];
} tJugadorAPI;

typedef struct {
    char* info;
    size_t tamInfo;
} tRespuesta;

typedef struct {
    int dia;
    int mes;
    int anio;
    int hora;
    int minutos;
    int segundos;
} tFechaHora;

void enviarDatosJSON(const void* elem1, const void* elem2);
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
CURLcode peticionGET(tRespuesta* respuesta, const char* path);
CURLcode peticionPOST(tRespuesta* respuesta, const char* pathUrl, const char* jsonData);

int parsearJugadores(tRespuesta*, tJugadorAPI*);
void mostrarJugadorAPI(const void*, const void*);
int compararFechaHora(const char*, const char*);
int compararJugAPI(const void*, const void*);

#endif // APIHEADER_H_INCLUDED
