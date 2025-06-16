#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void trim(char *s)
{
    // Con esto sacamos espacios al inicio y al final
    char *start = s, *end;
    while (isspace((unsigned char)*start))
        start++;
    if (start != s)
        memmove(s, start, strlen(start) + 1);
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end))
        *end-- = '\0';
}

int leerConfig(const char *ruta, tConfig *cfg)
{
    FILE *archConfig = fopen(ruta, "r");
    if (!archConfig)
        return 0;

    char line[512];

    // Línea 1: URL | CLAVE
    if (!fgets(line, sizeof(line), archConfig))
    {
        fclose(archConfig);
        return 0;
    }
    char *sep = strchr(line, '|');
    if (!sep)
    {
        fclose(archConfig);
        return 0;
    }
    *sep = '\0';
    strncpy(cfg->api_url, line, URL_LEN-1);
    cfg->api_url[URL_LEN-1] = '\0';
    strncpy(cfg->api_key, sep + 1, KEY_LEN-1);
    cfg->api_key[KEY_LEN-1] = '\0';
    trim(cfg->api_url);
    trim(cfg->api_key);

    // Línea 2: modo = VALOR
    if (!fgets(line, sizeof(line), archConfig))
    {
        fclose(archConfig);
        return 0;
    }
    trim(line);
    strncpy(cfg->modo, line, MODO_LEN-1);
    cfg->modo[MODO_LEN-1] = '\0';

    // Línea 3: partidas
    if (!fgets(line, sizeof(line), archConfig))
    {
        fclose(archConfig);
        return 0;
    }
    cfg->partidas = atoi(line); // Con atoi pasamos de char a int

    fclose(archConfig);
    return 1;
}
