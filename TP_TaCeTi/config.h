#ifndef CONFIG_H
#define CONFIG_H

#define MODO_LEN 16

typedef struct {
    char modo[MODO_LEN];
    int partidas;
} tConfig;

// devuelve 1 si pudo leer y parsear sin errores, 0 de lo contrario
int leerConfig(const char *ruta, tConfig *cfg);

#endif // CONFIG_H
