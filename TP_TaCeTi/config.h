#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#define MODO_LEN 16
#define URL_LEN 256
#define KEY_LEN 64

// Configuración general leída desde config.txt
typedef struct {
    char api_url[URL_LEN];    // URL base de la API
    char api_key[KEY_LEN];    // Clave para GET
    char modo[MODO_LEN];      // FACIL, INTERMEDIO, IMPOSIBLE
    int partidas;             // Partidas por jugador
} tConfig;


// Lee config.txt con formato:
//   linea1: <URL> | <KEY>
//   linea2: modo = <FACIL|INTERMEDIO|IMPOSIBLE>
//   linea3: <numero de partidas>
// Devuelve 1 en éxito, 0 en fallo.
int leerConfig(const char *ruta, tConfig *cfg);
void trim(char *s); // Para borrar espacios
#endif // CONFIG_H_INCLUDED
