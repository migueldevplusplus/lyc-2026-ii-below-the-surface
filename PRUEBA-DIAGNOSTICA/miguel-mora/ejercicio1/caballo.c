/*
 * Ejercicio 1 - Validador de movimientos de caballo (C)
 *
 * Un salto de caballo es valido si una coordenada cambia en 1 y la otra en 2.
 * El programa se detiene en el primer movimiento incorrecto.
 *
 * Compilar: gcc -O2 -o caballo caballo.c
 * Uso:      ./caballo                      -> ejemplos y mediciones
 *           ./caballo a1 "Kc2 Kd4 Kb3"     -> entrada propia
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libgen.h>
#include <unistd.h>

#define REPETICIONES 10
#define POSICION_INICIAL_SECUENCIA "a2" /* el enunciado no la da: Kb4 es alcanzable desde a2 */

typedef struct {
    int columna;
    int fila;
} Coordenada;

/* Convierte "c2" en {3, 2}. Devuelve 0 si la casilla no existe. */
static int casilla_a_coordenada(const char *casilla, size_t longitud, Coordenada *salida) {
    if (longitud != 2) return 0;
    if (casilla[0] < 'a' || casilla[0] > 'h') return 0;
    if (casilla[1] < '1' || casilla[1] > '8') return 0;
    salida->columna = casilla[0] - 'a' + 1;
    salida->fila = casilla[1] - '0';
    return 1;
}

static int es_salto_de_caballo(Coordenada origen, Coordenada destino) {
    int diferencia_columnas = abs(origen.columna - destino.columna);
    int diferencia_filas = abs(origen.fila - destino.fila);
    return (diferencia_columnas == 1 && diferencia_filas == 2) ||
           (diferencia_columnas == 2 && diferencia_filas == 1);
}

/* Escribe el resultado en 'resultado'. Recorre la cadena sin copiarla. */
static void validar_movimientos(const char *posicion_inicial, const char *cadena,
                                char *resultado, size_t tamano_resultado) {
    Coordenada posicion_actual, destino;
    if (!casilla_a_coordenada(posicion_inicial, strlen(posicion_inicial), &posicion_actual)) {
        snprintf(resultado, tamano_resultado, "INCORRECTO posicion inicial %s no existe", posicion_inicial);
        return;
    }

    const char *cursor = cadena;
    int numero_movimiento = 0;
    while (*cursor) {
        while (*cursor == ' ' || *cursor == '\n' || *cursor == '\t') cursor++;
        if (!*cursor) break;

        const char *inicio_movimiento = cursor;
        while (*cursor && *cursor != ' ' && *cursor != '\n' && *cursor != '\t') cursor++;
        size_t longitud_movimiento = (size_t)(cursor - inicio_movimiento);
        numero_movimiento++;

        if (inicio_movimiento[0] != 'K') {
            snprintf(resultado, tamano_resultado,
                     "movimiento INCORRECTO falla letra %c no pertenece a un caballo (movimiento %d)",
                     inicio_movimiento[0], numero_movimiento);
            return;
        }
        if (!casilla_a_coordenada(inicio_movimiento + 1, longitud_movimiento - 1, &destino)) {
            snprintf(resultado, tamano_resultado,
                     "INCORRECTO falla en el movimiento %d (casilla fuera del tablero)", numero_movimiento);
            return;
        }
        if (!es_salto_de_caballo(posicion_actual, destino)) {
            snprintf(resultado, tamano_resultado, "INCORRECTO falla en el movimiento %d", numero_movimiento);
            return;
        }
        posicion_actual = destino;
    }
    snprintf(resultado, tamano_resultado, "movimiento CORRECTO");
}

static int contar_movimientos(const char *cadena) {
    int cantidad = 0, dentro_de_palabra = 0;
    for (; *cadena; cadena++) {
        if (*cadena == ' ' || *cadena == '\n' || *cadena == '\t') dentro_de_palabra = 0;
        else if (!dentro_de_palabra) { dentro_de_palabra = 1; cantidad++; }
    }
    return cantidad;
}

/* Lee todo el archivo en memoria dinamica */
static char *leer_archivo(const char *ruta) {
    FILE *archivo = fopen(ruta, "r");
    if (!archivo) return NULL;
    fseek(archivo, 0, SEEK_END);
    long tamano = ftell(archivo);
    rewind(archivo);
    char *contenido = malloc((size_t)tamano + 1);
    size_t leidos = fread(contenido, 1, (size_t)tamano, archivo);
    contenido[leidos] = '\0';
    fclose(archivo);
    return contenido;
}

/* Copia los primeros 'cantidad' movimientos de la secuencia */
static char *primeros_movimientos(const char *cadena, int cantidad) {
    char *copia = malloc(strlen(cadena) + 1);
    int contados = 0, dentro_de_palabra = 0;
    size_t i = 0;
    for (; *cadena; cadena++) {
        int es_espacio = (*cadena == ' ' || *cadena == '\n' || *cadena == '\t');
        if (!es_espacio && !dentro_de_palabra) {
            if (contados == cantidad) break;
            contados++;
        }
        dentro_de_palabra = !es_espacio;
        copia[i++] = es_espacio ? ' ' : *cadena;
    }
    copia[i] = '\0';
    return copia;
}

static double segundos_transcurridos(struct timespec inicio, struct timespec fin) {
    return (double)(fin.tv_sec - inicio.tv_sec) + (double)(fin.tv_nsec - inicio.tv_nsec) / 1e9;
}

int main(int argc, char *argv[]) {
    /* Trabaja en la carpeta del ejecutable, para encontrar sus archivos desde donde sea */
    char ruta_programa[1024];
    snprintf(ruta_programa, sizeof ruta_programa, "%s", argv[0]);
    if (chdir(dirname(ruta_programa)) != 0) perror("chdir");

    char resultado[200];

    if (argc == 3) {
        validar_movimientos(argv[1], argv[2], resultado, sizeof resultado);
        printf("%s\n", resultado);
        return 0;
    }

    const char *ejemplos[][2] = {
        {"a1", "Kc2 Kd4 Kb3 Kc5"},
        {"a1", "Kc2 Kd4 Kb4 Kc5"},
        {"a2", "Kc3 Ke4 Kf6 Ke8 Kg7"},
        {"a2", "Kc8 Ke4 Kf6 Ke8 Kg7"},
        {"a8", "Kb6 Kd7"},
        {"a8", "Kb6 Kb7"},
        {"a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4"},
        {"a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4"},
        {"a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4"},
    };
    int cantidad_ejemplos = (int)(sizeof ejemplos / sizeof ejemplos[0]);

    FILE *salida = fopen("resultados_c.txt", "w");
    FILE *destinos[2] = {stdout, salida};

    for (int d = 0; d < 2; d++)
        fprintf(destinos[d], "=== Ejercicio 1 - Validador de caballo (C) ===\n\n");

    for (int i = 0; i < cantidad_ejemplos; i++) {
        validar_movimientos(ejemplos[i][0], ejemplos[i][1], resultado, sizeof resultado);
        for (int d = 0; d < 2; d++) {
            fprintf(destinos[d], "Ak=%s n=%d C=%s\n", ejemplos[i][0],
                    contar_movimientos(ejemplos[i][1]), ejemplos[i][1]);
            fprintf(destinos[d], "  salida: %s\n", resultado);
        }
    }

    char *secuencia = leer_archivo("secuencia.txt");
    if (!secuencia) {
        fprintf(stderr, "No se encontro secuencia.txt\n");
        return 1;
    }
    char *secuencia_100 = primeros_movimientos(secuencia, 100);
    struct timespec inicio, fin;
    char resultado_100[200];

    clock_gettime(CLOCK_MONOTONIC, &inicio);
    validar_movimientos(POSICION_INICIAL_SECUENCIA, secuencia_100, resultado_100, sizeof resultado_100);
    clock_gettime(CLOCK_MONOTONIC, &fin);
    double tiempo_100 = segundos_transcurridos(inicio, fin);

    clock_gettime(CLOCK_MONOTONIC, &inicio);
    for (int repeticion = 0; repeticion < REPETICIONES; repeticion++)
        validar_movimientos(POSICION_INICIAL_SECUENCIA, secuencia, resultado, sizeof resultado);
    clock_gettime(CLOCK_MONOTONIC, &fin);
    double tiempo_completo = segundos_transcurridos(inicio, fin);

    for (int d = 0; d < 2; d++) {
        fprintf(destinos[d], "\nPosicion inicial asumida para la secuencia: %s\n", POSICION_INICIAL_SECUENCIA);
        fprintf(destinos[d], "n = 100 movimientos: %s\n", resultado_100);
        fprintf(destinos[d], "  tiempo: %.2f microsegundos\n", tiempo_100 * 1e6);
        fprintf(destinos[d], "Secuencia del enunciado (%d movimientos) x %d: %s\n",
                contar_movimientos(secuencia), REPETICIONES, resultado);
        fprintf(destinos[d], "  tiempo total: %.2f microsegundos\n", tiempo_completo * 1e6);
    }

    fclose(salida);
    free(secuencia);
    free(secuencia_100);
    return 0;
}
