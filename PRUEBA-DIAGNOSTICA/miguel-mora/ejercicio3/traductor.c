/*
 * Ejercicio 3 - Traductor de palabras reservadas de C al espanol (C)
 *
 * 1. Carga el programa C completo en un bloque de memoria dinamica (malloc).
 * 2. Lo recorre caracter por caracter como un analizador lexico sencillo:
 *    - comentarios (// y / * * /), cadenas "..." y caracteres '...' se copian tal cual;
 *    - cada identificador se compara completo contra la tabla de palabras reservadas,
 *      asi "intentos" no se traduce aunque contenga "int".
 * 3. Escribe la traduccion en un buffer dinamico que crece con realloc,
 *    porque las palabras en espanol suelen ser mas largas.
 *
 * Compilar: gcc -O2 -o traductor traductor.c
 * Uso:      ./traductor               -> traduce ejemplo.c y mide tiempos
 *           ./traductor archivo.c     -> traduce otro archivo
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <libgen.h>
#include <unistd.h>

#define REPETICIONES 100

typedef struct {
    const char *palabra_c;
    const char *traduccion;
} PalabraReservada;

/* Las 32 palabras reservadas del estandar C89/C90 */
static const PalabraReservada PALABRAS_RESERVADAS[] = {
    {"auto", "automatico"}, {"break", "romper"}, {"case", "caso"}, {"char", "caracter"},
    {"const", "constante"}, {"continue", "continuar"}, {"default", "por_defecto"}, {"do", "hacer"},
    {"double", "doble"}, {"else", "sino"}, {"enum", "enumeracion"}, {"extern", "externo"},
    {"float", "flotante"}, {"for", "para"}, {"goto", "ir_a"}, {"if", "si"},
    {"int", "entero"}, {"long", "largo"}, {"register", "registro"}, {"return", "retornar"},
    {"short", "corto"}, {"signed", "con_signo"}, {"sizeof", "tamano_de"}, {"static", "estatico"},
    {"struct", "estructura"}, {"switch", "segun"}, {"typedef", "definir_tipo"}, {"union", "union"},
    {"unsigned", "sin_signo"}, {"void", "vacio"}, {"volatile", "volatil"}, {"while", "mientras"},
};
#define CANTIDAD_PALABRAS ((int)(sizeof PALABRAS_RESERVADAS / sizeof PALABRAS_RESERVADAS[0]))

/* Buffer de salida que crece segun haga falta */
typedef struct {
    char *contenido;
    size_t longitud;
    size_t capacidad;
} Buffer;

static void agregar(Buffer *buffer, const char *texto, size_t cantidad) {
    if (buffer->longitud + cantidad + 1 > buffer->capacidad) {
        while (buffer->longitud + cantidad + 1 > buffer->capacidad) buffer->capacidad *= 2;
        buffer->contenido = realloc(buffer->contenido, buffer->capacidad);
    }
    memcpy(buffer->contenido + buffer->longitud, texto, cantidad);
    buffer->longitud += cantidad;
    buffer->contenido[buffer->longitud] = '\0';
}

/* Devuelve el indice en la tabla o -1 si no es palabra reservada */
static int buscar_palabra(const char *inicio, size_t longitud) {
    for (int i = 0; i < CANTIDAD_PALABRAS; i++) {
        if (strlen(PALABRAS_RESERVADAS[i].palabra_c) == longitud &&
            strncmp(PALABRAS_RESERVADAS[i].palabra_c, inicio, longitud) == 0)
            return i;
    }
    return -1;
}

/* Traduce 'codigo'; cuenta las apariciones en 'conteo_por_palabra' */
static char *traducir(const char *codigo, int conteo_por_palabra[]) {
    Buffer salida = {malloc(256), 0, 256};
    salida.contenido[0] = '\0';
    size_t longitud = strlen(codigo);
    size_t posicion = 0;

    for (int i = 0; i < CANTIDAD_PALABRAS; i++) conteo_por_palabra[i] = 0;

    while (posicion < longitud) {
        char caracter = codigo[posicion];
        char siguiente = posicion + 1 < longitud ? codigo[posicion + 1] : '\0';
        size_t fin;

        if (caracter == '/' && siguiente == '/') {                 /* comentario de linea */
            const char *salto = strchr(codigo + posicion, '\n');
            fin = salto ? (size_t)(salto - codigo) : longitud;
            agregar(&salida, codigo + posicion, fin - posicion);
        } else if (caracter == '/' && siguiente == '*') {          /* comentario de bloque */
            const char *cierre = strstr(codigo + posicion + 2, "*/");
            fin = cierre ? (size_t)(cierre - codigo) + 2 : longitud;
            agregar(&salida, codigo + posicion, fin - posicion);
        } else if (caracter == '"' || caracter == '\'') {          /* cadena o caracter */
            fin = posicion + 1;
            while (fin < longitud && codigo[fin] != caracter)
                fin += (codigo[fin] == '\\') ? 2 : 1;
            fin = fin + 1 < longitud ? fin + 1 : longitud;
            agregar(&salida, codigo + posicion, fin - posicion);
        } else if (isalpha((unsigned char)caracter) || caracter == '_') { /* identificador */
            fin = posicion + 1;
            while (fin < longitud && (isalnum((unsigned char)codigo[fin]) || codigo[fin] == '_')) fin++;
            int indice = buscar_palabra(codigo + posicion, fin - posicion);
            if (indice >= 0) {
                conteo_por_palabra[indice]++;
                agregar(&salida, PALABRAS_RESERVADAS[indice].traduccion,
                        strlen(PALABRAS_RESERVADAS[indice].traduccion));
            } else {
                agregar(&salida, codigo + posicion, fin - posicion);
            }
        } else {                                                    /* cualquier otro simbolo */
            fin = posicion + 1;
            agregar(&salida, codigo + posicion, 1);
        }
        posicion = fin;
    }
    return salida.contenido;
}

/* Carga el archivo completo en memoria dinamica */
static char *cargar_archivo(const char *ruta, long *tamano) {
    FILE *archivo = fopen(ruta, "rb");
    if (!archivo) return NULL;
    fseek(archivo, 0, SEEK_END);
    *tamano = ftell(archivo);
    rewind(archivo);
    char *contenido = malloc((size_t)*tamano + 1);
    size_t leidos = fread(contenido, 1, (size_t)*tamano, archivo);
    contenido[leidos] = '\0';
    fclose(archivo);
    return contenido;
}

static int comparar_indices(const void *a, const void *b) {
    return strcmp(PALABRAS_RESERVADAS[*(const int *)a].palabra_c,
                  PALABRAS_RESERVADAS[*(const int *)b].palabra_c);
}

int main(int argc, char *argv[]) {
    /* Trabaja en la carpeta del ejecutable, para encontrar sus archivos desde donde sea */
    char ruta_programa[1024];
    snprintf(ruta_programa, sizeof ruta_programa, "%s", argv[0]);
    if (chdir(dirname(ruta_programa)) != 0) perror("chdir");

    const char *ruta_entrada = argc == 2 ? argv[1] : "ejemplo.c";
    long tamano;
    char *codigo = cargar_archivo(ruta_entrada, &tamano);
    if (!codigo) {
        fprintf(stderr, "No se pudo abrir %s\n", ruta_entrada);
        return 1;
    }

    int conteo_por_palabra[CANTIDAD_PALABRAS];
    char *codigo_traducido = traducir(codigo, conteo_por_palabra);

    int conteo_temporal[CANTIDAD_PALABRAS];
    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    for (int repeticion = 0; repeticion < REPETICIONES; repeticion++)
        free(traducir(codigo, conteo_temporal));
    clock_gettime(CLOCK_MONOTONIC, &fin);
    double tiempo_total = (double)(fin.tv_sec - inicio.tv_sec) + (double)(fin.tv_nsec - inicio.tv_nsec) / 1e9;

    int indices_encontrados[CANTIDAD_PALABRAS], cantidad_encontradas = 0, total_apariciones = 0;
    for (int i = 0; i < CANTIDAD_PALABRAS; i++) {
        if (conteo_por_palabra[i] > 0) {
            indices_encontrados[cantidad_encontradas++] = i;
            total_apariciones += conteo_por_palabra[i];
        }
    }
    qsort(indices_encontrados, (size_t)cantidad_encontradas, sizeof(int), comparar_indices);

    FILE *salida = fopen("resultados_c.txt", "w");
    FILE *destinos[2] = {stdout, salida};
    for (int d = 0; d < 2; d++) {
        fprintf(destinos[d], "=== Ejercicio 3 - Traductor de palabras reservadas (C) ===\n");
        fprintf(destinos[d], "Archivo: %s (%ld caracteres cargados en memoria)\n\n", ruta_entrada, tamano);
        fprintf(destinos[d], "Palabras reservadas encontradas:\n");
        for (int k = 0; k < cantidad_encontradas; k++) {
            int i = indices_encontrados[k];
            fprintf(destinos[d], "  %-10s -> %-14s (%d veces)\n", PALABRAS_RESERVADAS[i].palabra_c,
                    PALABRAS_RESERVADAS[i].traduccion, conteo_por_palabra[i]);
        }
        fprintf(destinos[d], "Total: %d apariciones de %d palabras distintas\n\n", total_apariciones, cantidad_encontradas);
        fprintf(destinos[d], "Tiempo de traducir el archivo %d veces (n=100): %.2f microsegundos\n\n",
                REPETICIONES, tiempo_total * 1e6);
        fprintf(destinos[d], "--- Codigo traducido ---\n%s\n", codigo_traducido);
    }

    fclose(salida);
    free(codigo);
    free(codigo_traducido);
    return 0;
}
