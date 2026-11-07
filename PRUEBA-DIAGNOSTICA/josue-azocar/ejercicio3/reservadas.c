/*
 * Ejercicio 3 - Detectar palabras reservadas de C y traducirlas al español.
 *
 *  1) Carga un .c en memoria dinamica (malloc).
 *  2) Lista reservadas con su traduccion.
 *  3) Muestra el codigo con reservadas sustituidas al español.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_PALABRA 64
#define MAX_RUTA 512
#define ANCHO_TABLA 72
#define ARCHIVO_POR_DEFECTO "archivo_codigo_ingles.c"
#define ARCHIVO_TRADUCIDO "codigo_traducido.c"

#define MODO_LISTAR 0
#define MODO_TRADUCIR 1

typedef struct {
    const char *ingles;
    const char *espanol;
} Traduccion;

static const Traduccion DICCIONARIO[] = {
    {"auto", "automatico"}, {"break", "romper"}, {"case", "caso"},
    {"char", "caracter"}, {"const", "constante"}, {"continue", "continuar"},
    {"default", "por_defecto"}, {"do", "hacer"}, {"double", "doble"},
    {"else", "si_no"}, {"enum", "enumeracion"}, {"extern", "externo"},
    {"float", "flotante"}, {"for", "para"}, {"goto", "ir_a"},
    {"if", "si"}, {"int", "entero"}, {"long", "largo"},
    {"register", "registro"}, {"return", "retornar"}, {"short", "corto"},
    {"signed", "con_signo"}, {"sizeof", "tamano_de"}, {"static", "estatico"},
    {"struct", "estructura"}, {"switch", "segun"}, {"typedef", "definir_tipo"},
    {"union", "union"}, {"unsigned", "sin_signo"}, {"void", "vacio"},
    {"volatile", "volatil"}, {"while", "mientras"},
};

static const int CANTIDAD_RESERVADAS =
    (int)(sizeof(DICCIONARIO) / sizeof(DICCIONARIO[0]));

static void imprimir_linea(char caracter) {
    int i;
    for (i = 0; i < ANCHO_TABLA; i++) putchar(caracter);
    putchar('\n');
}

static void imprimir_titulo(const char *titulo) {
    printf("\n");
    imprimir_linea('=');
    printf("  %s\n", titulo);
    imprimir_linea('=');
}

static void limpiar_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static void ruta_junto_al_exe(const char *nombre, char *salida, size_t tamano);
static const char *traducir(const char *palabra) {
    int i;
    for (i = 0; i < CANTIDAD_RESERVADAS; i++) {
        if (strcmp(DICCIONARIO[i].ingles, palabra) == 0) {
            return DICCIONARIO[i].espanol;
        }
    }
    return NULL;
}

static int es_inicio_identificador(char c) {
    return isalpha((unsigned char)c) || c == '_';
}

static int es_parte_identificador(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

static int cargar_archivo_dinamico(const char *ruta, char **salida, long *tamano) {
    FILE *archivo;
    char *buffer;
    long bytes;

    archivo = fopen(ruta, "rb");
    if (!archivo) {
        printf("No se pudo abrir el archivo: %s\n", ruta);
        return 0;
    }

    if (fseek(archivo, 0, SEEK_END) != 0) {
        fclose(archivo);
        printf("Error al medir el archivo.\n");
        return 0;
    }

    bytes = ftell(archivo);
    if (bytes < 0) {
        fclose(archivo);
        printf("Error al obtener el tamano del archivo.\n");
        return 0;
    }
    rewind(archivo);

    buffer = (char *)malloc((size_t)bytes + 1);
    if (!buffer) {
        fclose(archivo);
        printf("No hay memoria suficiente para cargar el archivo.\n");
        return 0;
    }

    if (bytes > 0 && fread(buffer, 1, (size_t)bytes, archivo) != (size_t)bytes) {
        free(buffer);
        fclose(archivo);
        printf("Error al leer el archivo.\n");
        return 0;
    }

    buffer[bytes] = '\0';
    fclose(archivo);

    /* Quitar CR de CRLF (Windows) para no duplicar saltos al guardar. */
    {
        long lectura = 0;
        long escritura = 0;
        for (lectura = 0; lectura < bytes; lectura++) {
            if (buffer[lectura] != '\r') {
                buffer[escritura++] = buffer[lectura];
            }
        }
        buffer[escritura] = '\0';
        bytes = escritura;
    }

    *salida = buffer;
    *tamano = bytes;
    return 1;
}

/* Avanza sobre comentarios o literales; si no aplica, deja i igual. */
static long saltar_no_codigo(const char *codigo, long i) {
    if (codigo[i] == '/' && codigo[i + 1] == '/') {
        i += 2;
        while (codigo[i] != '\0' && codigo[i] != '\n') i++;
        return i;
    }
    if (codigo[i] == '/' && codigo[i + 1] == '*') {
        i += 2;
        while (codigo[i] != '\0' && !(codigo[i] == '*' && codigo[i + 1] == '/')) i++;
        if (codigo[i] != '\0') i += 2;
        return i;
    }
    if (codigo[i] == '"' || codigo[i] == '\'') {
        char delim = codigo[i++];
        while (codigo[i] != '\0') {
            if (codigo[i] == '\\' && codigo[i + 1] != '\0') {
                i += 2;
                continue;
            }
            if (codigo[i] == delim) {
                i++;
                break;
            }
            i++;
        }
        return i;
    }
    return i;
}

/* Lee un identificador en palabra[]; avanza *pos. */
static void leer_identificador(const char *codigo, long *pos, char *palabra) {
    int largo = 0;
    while (es_parte_identificador(codigo[*pos]) && largo < MAX_PALABRA - 1) {
        palabra[largo++] = codigo[(*pos)++];
    }
    palabra[largo] = '\0';
    while (es_parte_identificador(codigo[*pos])) (*pos)++;
}

/*
 * Escribe el codigo traducido en el FILE indicado (consola o archivo .c).
 * No imprime titulos; solo el contenido traducido.
 */
static void escribir_codigo_traducido(const char *codigo, FILE *salida) {
    long i = 0;

    while (codigo[i] != '\0') {
        long siguiente = saltar_no_codigo(codigo, i);

        if (siguiente != i) {
            while (i < siguiente) fputc(codigo[i++], salida);
            continue;
        }

        if (es_inicio_identificador(codigo[i])) {
            char palabra[MAX_PALABRA];
            const char *espanol;

            leer_identificador(codigo, &i, palabra);
            espanol = traducir(palabra);
            fputs(espanol ? espanol : palabra, salida);
            continue;
        }

        fputc(codigo[i++], salida);
    }
}

/*
 * Un solo recorrido del codigo para listar o para imprimir traducido.
 */
static void recorrer_codigo(const char *codigo, int modo) {
    long i = 0;
    int encontrados = 0;

    if (modo == MODO_LISTAR) {
        imprimir_titulo("PALABRAS RESERVADAS DETECTADAS Y SU TRADUCCION");
        printf("%-6s%-16s%-24s\n", "No.", "Reservada (C)", "Traduccion (ES)");
        imprimir_linea('-');
    } else {
        imprimir_titulo("CODIGO CON PALABRAS RESERVADAS TRADUCIDAS AL ESPANOL");
        escribir_codigo_traducido(codigo, stdout);
        imprimir_linea('=');
        return;
    }

    while (codigo[i] != '\0') {
        long siguiente = saltar_no_codigo(codigo, i);

        if (siguiente != i) {
            i = siguiente;
            continue;
        }

        if (es_inicio_identificador(codigo[i])) {
            char palabra[MAX_PALABRA];
            const char *espanol;

            leer_identificador(codigo, &i, palabra);
            espanol = traducir(palabra);
            if (espanol) {
                encontrados++;
                printf("%-6d%-16s%-24s\n", encontrados, palabra, espanol);
            }
            continue;
        }

        i++;
    }

    imprimir_linea('-');
    printf("  Total encontradas: %d\n", encontrados);
    imprimir_linea('=');
}

/* Guarda una copia .c del codigo ya traducido en la carpeta del ejercicio. */
static int guardar_codigo_traducido(const char *codigo) {
    char ruta[MAX_RUTA];
    FILE *archivo;

    ruta_junto_al_exe(ARCHIVO_TRADUCIDO, ruta, sizeof(ruta));
    archivo = fopen(ruta, "w");
    if (!archivo) {
        printf("No se pudo crear el archivo: %s\n", ruta);
        return 0;
    }

    fprintf(archivo,
            "/* Copia traducida generada por el Ejercicio 3 */\n"
            "/* Palabras reservadas de C sustituidas por su equivalente en espanol */\n\n");
    escribir_codigo_traducido(codigo, archivo);
    fclose(archivo);

    printf("Codigo traducido guardado en: %s\n", ruta);
    return 1;
}
static void mostrar_diccionario(void) {
    int i;
    imprimir_titulo("DICCIONARIO DE PALABRAS RESERVADAS (C -> Espanol)");
    printf("%-16s%s\n", "C (ingles)", "Espanol");
    imprimir_linea('-');
    for (i = 0; i < CANTIDAD_RESERVADAS; i++) {
        printf("%-16s%s\n", DICCIONARIO[i].ingles, DICCIONARIO[i].espanol);
    }
    imprimir_linea('=');
    printf("  Total en diccionario: %d\n", CANTIDAD_RESERVADAS);
}

static void ruta_junto_al_exe(const char *nombre, char *salida, size_t tamano) {
#ifdef _WIN32
    char ruta_exe[MAX_RUTA];
    char *barra;
    DWORD leidos = GetModuleFileNameA(NULL, ruta_exe, MAX_RUTA);

    if (leidos > 0 && leidos < MAX_RUTA) {
        barra = strrchr(ruta_exe, '\\');
        if (!barra) barra = strrchr(ruta_exe, '/');
        if (barra) {
            *(barra + 1) = '\0';
            if (strlen(ruta_exe) + strlen(nombre) + 1 < tamano) {
                sprintf(salida, "%s%s", ruta_exe, nombre);
                return;
            }
        }
    }
#endif
    strncpy(salida, nombre, tamano - 1);
    salida[tamano - 1] = '\0';
}

static void analizar_archivo(const char *ruta) {
    char *codigo = NULL;
    long tamano = 0;

    printf("\nCargando en memoria dinamica: %s\n", ruta);
    if (!cargar_archivo_dinamico(ruta, &codigo, &tamano)) return;

    printf("Archivo cargado correctamente.\n");
    printf("Bytes en memoria (malloc): %ld\n", tamano);

    recorrer_codigo(codigo, MODO_LISTAR);
    recorrer_codigo(codigo, MODO_TRADUCIR);
    guardar_codigo_traducido(codigo);

    free(codigo);
    printf("Memoria liberada (free).\n");
}

int main(void) {
    int opcion;
    char ruta[MAX_RUTA];
    char ruta_defecto[MAX_RUTA];

    for (;;) {
        printf("\n===== EJERCICIO 3: Palabras reservadas de C =====\n");
        printf("1) Analizar y traducir archivo por defecto (%s)\n", ARCHIVO_POR_DEFECTO);
        printf("2) Analizar y traducir otro archivo .c\n");
        printf("3) Ver diccionario completo\n");
        printf("0) Salir\n");
        printf("Opcion: ");

        if (scanf("%d", &opcion) != 1) {
            limpiar_entrada();
            printf("Opcion no valida.\n");
            continue;
        }
        limpiar_entrada();

        if (opcion == 1) {
            ruta_junto_al_exe(ARCHIVO_POR_DEFECTO, ruta_defecto, sizeof(ruta_defecto));
            analizar_archivo(ruta_defecto);
        } else if (opcion == 2) {
            size_t len;
            printf("Ruta del archivo .c: ");
            if (!fgets(ruta, (int)sizeof(ruta), stdin)) {
                printf("No se pudo leer la ruta.\n");
                continue;
            }
            len = strlen(ruta);
            if (len > 0 && ruta[len - 1] == '\n') ruta[len - 1] = '\0';
            if (ruta[0] == '\0') {
                printf("Ruta vacia.\n");
                continue;
            }
            analizar_archivo(ruta);
        } else if (opcion == 3) {
            mostrar_diccionario();
        } else if (opcion == 0) {
            printf("Fin del programa.\n");
            break;
        } else {
            printf("Opcion no valida.\n");
        }
    }
    return 0;
}
