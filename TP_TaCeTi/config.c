#include "config.h"
#include "ini.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

static int config_handler(void *user, const char *section,
                          const char *name, const char *value)
{
    tConfig *cfg = (tConfig*)user;

    if (strcmp(section, "dificultad") == 0 && strcmp(name, "modo") == 0)
    {
        // copiar modo (hasta MODO_LEN-1 chars)
        strncpy(cfg->modo, value, MODO_LEN-1);
        cfg->modo[MODO_LEN-1] = '\0';
        char *p = strchr(cfg->modo, ';');
        if (p) *p = '\0';
        size_t L = strlen(cfg->modo);
        while (L > 0 && isspace((unsigned char)cfg->modo[L-1]))
        {
            cfg->modo[L-1] = '\0';
            --L;
        }
    }
    else if (strcmp(section, "partidas") == 0 && strcmp(name, "por_jugador") == 0)
    {
        cfg->partidas = atoi(value);
    }
    return 1;  // retorno 0 detendría el parseo
}

int leerConfig(const char *ruta, tConfig *cfg)
{
    // valores por defecto (en caso de faltar secciones/claves)
    cfg->modo[0] = '\0';
    cfg->partidas = 0;

    int error = ini_parse(ruta, config_handler, cfg);
    if (error == 0)
    {
        // éxito
        return 1;
    }
    else
    {
        // error: ini_parse devuelve
        //   -1 = no pudo abrir el archivo
        //   n >= 1 = línea con error de parseo
        fprintf(stderr, "Error parseando '%s' en línea %d\n", ruta, error);
        return 0;
    }
}
