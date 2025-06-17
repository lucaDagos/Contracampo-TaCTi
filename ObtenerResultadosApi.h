/*#ifndef API_H_INCLUDED
#define API_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curl/curl.h>

#define TAM_MAX_JSON 200
#define URL "https://algoritmos-api.azurewebsites.net/api/TaCTi/%7Bestructura%7Dasdasd"

typedef struct{
            char nombre[30];
            int puntaje;
            time_t FechayHora;
            }Resultado;
typedef struct {
    char* info;
    size_t tamInfo;
} tRespuesta;
void obtenerResultadosApi(const char* urlApi);
void leerResultadosApi();
CURLcode peticionGET(tRespuesta* respuesta, const char* path);
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
#endif // API_H_INCLUDED
*/