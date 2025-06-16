#ifndef API_H_INCLUDED
#define API_H_INCLUDED
#include <curl/curl.h>
#include "cJSON.h"
#include "stdio.h"
#include "stdlib.h"
#include "lista.h"
#include "taceti.h"
#include "config.h"

void enviarResultados(tLista* lista, const tConfig *cfg);
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp);
void hacerGET(const tConfig *cfg);
void mostrarNombresYPuntos(const char* json);


#endif // API_H_INCLUDED
