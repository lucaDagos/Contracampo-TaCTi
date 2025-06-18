#include "API.h"

CURLcode peticionGET(tRespuesta* respuesta, const char* path){

    long codigoHTTP = 0;
    CURLcode res;
    CURL* curl;
    // Inicializar el manejo de curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (!curl)
        return CURLE_FAILED_INIT;

    // Establecer la URL de la solicitud GET
    curl_easy_setopt(curl, CURLOPT_URL, path);

    // Establecer la función de retorno de llamada para manejar la respuesta
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, respuesta);

    // Realizar la solicitud HTTP GET
    res = curl_easy_perform(curl);

    if (res == CURLE_OK)
    {
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &codigoHTTP);
        printf("Codigo HTTP recibido: %ld\n", codigoHTTP);

        if (codigoHTTP == 200) {
            printf("La API respondio 200 OK\n");
        } else if (codigoHTTP == 400) {
            printf("La API respondio 400 Bad Request\n");
        } else {
            printf("La API respondio otro codigo: %ld\n", codigoHTTP);
        }
    } else
    {
        fprintf(stderr, "Error en la solicitud: %s\n", curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return res;

}
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp){

    // Función que maneja la respuesta de la solicitud HTTP
    size_t realsize = size* nmemb;
    tRespuesta* res = (tRespuesta*) userp;
    char* info = malloc(realsize + 1);

    if(!info){
        printf("No hay memoria suficiente");
        return 0;
    }

    memcpy(info, (char*) contents, (int) realsize);
    *(info + realsize) = '\0';
    res->info = info;
    res->tamInfo = realsize;

    return realsize;

}

void obtenerResultadosApi(const char* urlApi)
{
    char obtenerApi[TAM_MAX_JSON];
    tRespuesta respuesta;
    snprintf(obtenerApi, sizeof(obtenerApi),
             "curl -s -X GET \"%s\" -o respuesta_get.json",
             urlApi);

    int resultado = system(obtenerApi);
    peticionGET(&respuesta,urlApi);
    if (resultado != 0) {
        printf("Error al realizar la solicitud GET.\n");
    } else {
        printf("Resultados obtenidos y guardados en 'respuesta_get.json'.\n");
    }
}
void leerResultadosApi()
{
    char *data;
    long length;
    FILE* f = fopen("respuesta_get.json", "rb");
    if (!f) {
        printf("\nNo se pudo abrir el archivo JSON");
        return;
    }
    //busco la maxima longitud del archivo
    fseek(f,0,SEEK_END);
    length = ftell(f);
    fseek(f, 0, SEEK_SET);

    data = (char*)malloc(length + 1);
    if (!data) {
        printf("\nNo se pudo reservar memoria.\n");
        fclose(f);
        return;
    }
    fread(data,1,length,f);
    data[length] = '\0';

    printf("\n%s",data);
    return;
}
