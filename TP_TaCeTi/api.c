#include "api.h"

void enviarResultados(tLista* lista)
{
    CURL *curl;
    CURLcode res;

    char json[1024] = "{\"CodigoGrupo\":\"puntero\",\"Jugadores\":[";
    tNodo *actual = *lista;
    int primero = 1;

    while (actual) {
        jugador *j = (jugador*)actual->dato;
        char jugadorJSON[256];
        snprintf(jugadorJSON, sizeof(jugadorJSON), "%s{\"nombre\":\"%s\",\"puntos\":%d}",
                 primero ? "" : ",", j->nombre, j->puntaje);
        strcat(json, jugadorJSON);
        primero = 0;
        actual = actual->sig;
    }

    strcat(json, "]}");

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, "https://algoritmos-api.azurewebsites.net/api/TaCTi");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json);

        // Desactiva verificación de certificado (no recomendado para producción)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "Error al enviar POST: %s\n", curl_easy_strerror(res));
        else
            printf("Resultados enviados correctamente.\n");

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    curl_global_cleanup();
}

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    char **response_ptr = (char**)userp;

    *response_ptr = realloc(*response_ptr, realsize + 1 + (*response_ptr ? strlen(*response_ptr) : 0));
    if (*response_ptr == NULL) return 0;

    if ((*response_ptr)[0] == '\0') {
        memcpy(*response_ptr, contents, realsize);
        (*response_ptr)[realsize] = '\0';
    } else {
        strcat(*response_ptr, contents);
    }

    return realsize;
}

void hacerGET() {
    CURL *curl;
    CURLcode res;
    char *response = calloc(1, 1); // Reservamos memoria inicial vacía

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://algoritmos-api.azurewebsites.net/api/TaCTi/puntero");

        //  Desactiva verificación SSL (opcional)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        // Configura el callback para recibir la respuesta
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
            fprintf(stderr, "Error al hacer GET: %s\n", curl_easy_strerror(res));
        else
        {
            //printf("Respuesta del servidor:\n%s\n", response);
            mostrarNombresYPuntos(response);
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    free(response);
}

void mostrarNombresYPuntos(const char* json)
{
    cJSON *root = cJSON_Parse(json);
    if (!root || !cJSON_IsArray(root)) {
        printf("Error al parsear JSON\n");
        return;
    }

    int n = cJSON_GetArraySize(root);
    for (int i = 0; i < n; i++) {
        cJSON *jugador = cJSON_GetArrayItem(root, i);
        cJSON *nombre = cJSON_GetObjectItemCaseSensitive(jugador, "nombreJugador");
        cJSON *puntaje = cJSON_GetObjectItemCaseSensitive(jugador, "puntaje");
        if (cJSON_IsString(nombre) && cJSON_IsNumber(puntaje)) {
            printf("Nombre: %s - Puntos: %d\n", nombre->valuestring, puntaje->valueint);
        }
    }

    cJSON_Delete(root);
}
