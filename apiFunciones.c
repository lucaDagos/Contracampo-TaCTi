#include "apiHeader.h"

void enviarDatosJSON(const void* elem1, const void* elem2){

    tJugador* jugador = (tJugador*) elem1;

    tConfiguracion* configuracion = (tConfiguracion*) elem2;

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

CURLcode peticionPOST(tRespuesta* respuesta, const char* pathUrl, const char* jsonData){

    CURLcode res;
    CURL* curl;
    struct curl_slist *headers = NULL;

    // Inicializar el manejo de curl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (!curl)
        return CURLE_FAILED_INIT;

    // Agregar encabezados para indicar que se envía JSON
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Configurar la URL
    curl_easy_setopt(curl, CURLOPT_URL, pathUrl);

    // Indicar que es una solicitud POST
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    // Enviar los datos JSON
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData);

    // Configurar los encabezados
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Establecer la función de retorno de llamada para manejar la respuesta
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, respuesta);


    // Realizar la solicitud HTTP GET
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return res;

}

// ------------------------------------------------------------

// Parsear la informacion que se obtiene de la api
int parsearJugadores(tRespuesta* res, tJugadorAPI* jugador){

    char *p = strrchr(res->info, '}');

    if(!p)
    {
        return 0;
    }

    p--;
    *p = '\0';

    p = strrchr(res->info, ',');
    p = strchr(p, ':');
    strcpy(jugador->fyh, p + 2);
    p = strrchr(res->info, ',');

    *p = '\0';
    p = strrchr(res->info, ',');
    p = strchr(p, ':');
    sscanf(p + 1, "%u", &jugador->puntaje);
    p = strrchr(res->info, ',');

    p--;
    *p = '\0';
    p = strrchr(res->info, '{');
    p = strchr(p, ':');
    strcpy(jugador->nombre, p + 2);
    p = strrchr(res->info, '{');

    *p = '\0';

    return 1;
}

// Se usa para mostrar los jugadores de la lista del ranking (posicion - jugador)
void mostrarJugadorAPI(const void* a, const void* b){

    tJugadorAPI* aa = (tJugadorAPI*) a;

    int posicion = *(int*) b;

    printf("| %-3d | %-15s | %-13d | %-20s |\n", posicion, aa->nombre, aa->puntaje, aa->fyh);
}

int compararFechaHora(const char* fyh1, const char* fyh2){

    tFechaHora f1, f2;

    sscanf(fyh1, "%2d/%2d/%4d %2d:%2d:%2d",
           &f1.dia, &f1.mes, &f1.anio,
           &f1.hora, &f1.minutos, &f1.segundos);

    sscanf(fyh2, "%2d/%2d/%4d %2d:%2d:%2d",
           &f2.dia, &f2.mes, &f2.anio,
           &f2.hora, &f2.minutos, &f2.segundos);

    int valores1[] = {f1.anio, f1.mes, f1.dia, f1.hora, f1.minutos, f1.segundos};
    int valores2[] = {f2.anio, f2.mes, f2.dia, f2.hora, f2.minutos, f2.segundos};

    for (int i = 0; i < 6; i++) {
        if (valores1[i] != valores2[i])
            return valores1[i] - valores2[i];
    }

    return 0;
}

// Comparar jugadores API segun Puntaje (descendente)
int compararJugAPI(const void* a, const void* b){
    const tJugadorAPI* jugA = (const tJugadorAPI*)a;
    const tJugadorAPI* jugB = (const tJugadorAPI*)b;

    if (jugA->puntaje < jugB->puntaje) return -1;
    if (jugA->puntaje > jugB->puntaje) return 1;
    return 0;
}


