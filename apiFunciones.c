#include "apiHeader.h"

void enviarDatosJSON(const void* elem1, const void* elem2){

    tJugador* jugador = (tJugador*) elem1;

    tConfiguracion* configuracion= (tConfiguracion*) elem2;

    char jsonData[TAM_MAX_JSON];

    tRespuesta respuesta;

    // Iniciar el JSON
    snprintf(jsonData, sizeof(jsonData), "{ \"CodigoGrupo\": \"%s\", \"Jugadores\": [{\"nombre\": \"%s\", \"puntos\": %d}]}",
             configuracion->codIdenGrupo,
             jugador->nombre,
             jugador->puntaje);

    peticionPOST(&respuesta, configuracion->urlApi, jsonData);

}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp){

    // Funci�n que maneja la respuesta de la solicitud HTTP
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

CURLcode peticionGET(tRespuesta* respuesta, const char* path){

    CURLcode res;
    CURL* curl;
    // Inicializar el manejo de curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (!curl)
        return CURLE_FAILED_INIT;

    // Establecer la URL de la solicitud GET
    curl_easy_setopt(curl, CURLOPT_URL, path);

    // Establecer la funci�n de retorno de llamada para manejar la respuesta
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, respuesta);

    // Realizar la solicitud HTTP GET
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return res;

}

CURLcode peticionPOST(tRespuesta* respuesta, const char* pathUrl, const char* jsonData){

    CURLcode res;
    CURL* curl;
    struct curl_slist *headers = NULL;

    // Inicializar el manejo de curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (!curl)
        return CURLE_FAILED_INIT;

    // Agregar encabezados para indicar que se env�a JSON
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Configurar la URL
    curl_easy_setopt(curl, CURLOPT_URL, pathUrl);

    // Indicar que es una solicitud POST
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    // Enviar los datos JSON
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData);

    // Configurar los encabezados
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Establecer la funci�n de retorno de llamada para manejar la respuesta
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, respuesta);


    // Realizar la solicitud HTTP GET
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return res;

}
