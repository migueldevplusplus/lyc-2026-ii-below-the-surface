/*
 * Ejercicio 2 - Coeficientes de (x+1)^n con el triangulo de Pascal (C)
 *
 * a) Construye el triangulo fila por fila con memoria dinamica (malloc):
 *    cada valor interior es la suma de los dos que tiene encima.
 * b) Evalua f(x) = (x+1)^n por pasos con Horner:
 *    acumulado = acumulado * x + coeficiente, desde el grado mas alto.
 *
 * Para n=100 los coeficientes llegan a ~10^29 y no caben en un long long
 * (maximo ~9.2 * 10^18). Por eso se usa un "numero grande": un arreglo
 * dinamico de bloques de 9 digitos (base 10^9), guardado de menor a mayor.
 *
 * Compilar: gcc -O2 -o pascal pascal.c
 * Uso:      ./pascal          -> demo n=5, x=2 y mediciones con n=100
 *           ./pascal 7 3      -> n=7, x=3
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <libgen.h>
#include <unistd.h>

#define BASE 1000000000u
#define REPETICIONES_MEDICION 1000

typedef struct {
    int cantidad_bloques;
    uint32_t *bloques; /* bloques[0] = 9 digitos menos significativos */
} NumeroGrande;

static NumeroGrande numero_desde_entero(uint32_t valor) {
    NumeroGrande numero;
    numero.cantidad_bloques = 1;
    numero.bloques = malloc(sizeof(uint32_t));
    numero.bloques[0] = valor;
    return numero;
}

static void liberar_numero(NumeroGrande *numero) {
    free(numero->bloques);
    numero->bloques = NULL;
    numero->cantidad_bloques = 0;
}

/* Suma bloque a bloque, como a mano, llevando el acarreo */
static NumeroGrande sumar(const NumeroGrande *a, const NumeroGrande *b) {
    int maximo = a->cantidad_bloques > b->cantidad_bloques ? a->cantidad_bloques : b->cantidad_bloques;
    NumeroGrande suma;
    suma.bloques = malloc((size_t)(maximo + 1) * sizeof(uint32_t));
    uint64_t acarreo = 0;
    int i;
    for (i = 0; i < maximo; i++) {
        uint64_t total = acarreo;
        if (i < a->cantidad_bloques) total += a->bloques[i];
        if (i < b->cantidad_bloques) total += b->bloques[i];
        suma.bloques[i] = (uint32_t)(total % BASE);
        acarreo = total / BASE;
    }
    if (acarreo) suma.bloques[i++] = (uint32_t)acarreo;
    suma.cantidad_bloques = i;
    return suma;
}

/* Multiplica un numero grande por un entero pequeno (x) */
static NumeroGrande multiplicar_por_entero(const NumeroGrande *a, uint32_t factor) {
    NumeroGrande producto;
    producto.bloques = malloc((size_t)(a->cantidad_bloques + 2) * sizeof(uint32_t));
    uint64_t acarreo = 0;
    int i;
    for (i = 0; i < a->cantidad_bloques; i++) {
        uint64_t total = (uint64_t)a->bloques[i] * factor + acarreo;
        producto.bloques[i] = (uint32_t)(total % BASE);
        acarreo = total / BASE;
    }
    while (acarreo) {
        producto.bloques[i++] = (uint32_t)(acarreo % BASE);
        acarreo /= BASE;
    }
    producto.cantidad_bloques = i;
    while (producto.cantidad_bloques > 1 && producto.bloques[producto.cantidad_bloques - 1] == 0)
        producto.cantidad_bloques--;
    return producto;
}

/* Devuelve el numero como texto (memoria dinamica, liberar despues) */
static char *numero_a_texto(const NumeroGrande *numero) {
    char *texto = malloc((size_t)numero->cantidad_bloques * 9 + 1);
    int posicion = sprintf(texto, "%u", numero->bloques[numero->cantidad_bloques - 1]);
    for (int i = numero->cantidad_bloques - 2; i >= 0; i--)
        posicion += sprintf(texto + posicion, "%09u", numero->bloques[i]);
    return texto;
}

/* Triangulo como matriz dinamica "escalonada": la fila k tiene k+1 elementos */
static NumeroGrande **generar_triangulo(int n) {
    NumeroGrande **triangulo = malloc((size_t)(n + 1) * sizeof(NumeroGrande *));
    for (int fila = 0; fila <= n; fila++) {
        triangulo[fila] = malloc((size_t)(fila + 1) * sizeof(NumeroGrande));
        triangulo[fila][0] = numero_desde_entero(1);
        if (fila > 0) triangulo[fila][fila] = numero_desde_entero(1);
        for (int posicion = 1; posicion < fila; posicion++)
            triangulo[fila][posicion] = sumar(&triangulo[fila - 1][posicion - 1],
                                              &triangulo[fila - 1][posicion]);
    }
    return triangulo;
}

static void liberar_triangulo(NumeroGrande **triangulo, int n) {
    for (int fila = 0; fila <= n; fila++) {
        for (int posicion = 0; posicion <= fila; posicion++)
            liberar_numero(&triangulo[fila][posicion]);
        free(triangulo[fila]);
    }
    free(triangulo);
}

static void imprimir_en_ambos(FILE *archivo, const char *formato, const char *texto) {
    printf(formato, texto);
    if (archivo) fprintf(archivo, formato, texto);
}

static void resolver(int n, uint32_t x, int mostrar_triangulo, FILE *archivo) {
    char linea[256];
    NumeroGrande **triangulo = generar_triangulo(n);

    if (mostrar_triangulo) {
        imprimir_en_ambos(archivo, "%s", "Triangulo de Pascal:\n");
        for (int fila = 0; fila <= n; fila++) {
            snprintf(linea, sizeof linea, "  (n=%d):", fila);
            imprimir_en_ambos(archivo, "%s", linea);
            for (int posicion = 0; posicion <= fila; posicion++) {
                char *valor = numero_a_texto(&triangulo[fila][posicion]);
                imprimir_en_ambos(archivo, " %s", valor);
                free(valor);
            }
            imprimir_en_ambos(archivo, "%s", "\n");
        }
    }

    NumeroGrande *coeficientes = triangulo[n];

    snprintf(linea, sizeof linea, "Coeficientes de (x+1)^%d:", n);
    imprimir_en_ambos(archivo, "%s", linea);
    for (int k = 0; k <= n; k++) {
        char *valor = numero_a_texto(&coeficientes[k]);
        imprimir_en_ambos(archivo, " %s", valor);
        free(valor);
    }
    imprimir_en_ambos(archivo, "%s", "\n");

    snprintf(linea, sizeof linea, "(x+1)^%d = ", n);
    imprimir_en_ambos(archivo, "%s", linea);
    for (int k = 0; k <= n; k++) {
        int exponente = n - k;
        char *valor = numero_a_texto(&coeficientes[k]);
        if (k > 0) imprimir_en_ambos(archivo, "%s", " + ");
        if (exponente == 0 || strcmp(valor, "1") != 0) imprimir_en_ambos(archivo, "%s", valor);
        if (exponente == 1) imprimir_en_ambos(archivo, "%s", "x");
        else if (exponente > 1) {
            snprintf(linea, sizeof linea, "x^%d", exponente);
            imprimir_en_ambos(archivo, "%s", linea);
        }
        free(valor);
    }
    imprimir_en_ambos(archivo, "%s", "\n");

    snprintf(linea, sizeof linea, "Evaluacion por pasos (Horner) con x = %u:\n", x);
    imprimir_en_ambos(archivo, "%s", linea);
    NumeroGrande acumulado = numero_desde_entero(0);
    for (int k = 0; k <= n; k++) {
        NumeroGrande multiplicado = multiplicar_por_entero(&acumulado, x);
        NumeroGrande nuevo_acumulado = sumar(&multiplicado, &coeficientes[k]);
        char *texto_anterior = numero_a_texto(&acumulado);
        char *texto_coeficiente = numero_a_texto(&coeficientes[k]);
        char *texto_nuevo = numero_a_texto(&nuevo_acumulado);
        snprintf(linea, sizeof linea, "  Paso %d (x^%d): ", k + 1, n - k);
        imprimir_en_ambos(archivo, "%s", linea);
        imprimir_en_ambos(archivo, "%s", texto_anterior);
        snprintf(linea, sizeof linea, " * %u + ", x);
        imprimir_en_ambos(archivo, "%s", linea);
        imprimir_en_ambos(archivo, "%s", texto_coeficiente);
        imprimir_en_ambos(archivo, " = %s\n", texto_nuevo);
        free(texto_anterior); free(texto_coeficiente); free(texto_nuevo);
        liberar_numero(&multiplicado);
        liberar_numero(&acumulado);
        acumulado = nuevo_acumulado;
    }

    /* Comprobacion: (x+1)^n multiplicando n veces */
    NumeroGrande potencia = numero_desde_entero(1);
    for (int i = 0; i < n; i++) {
        NumeroGrande siguiente = multiplicar_por_entero(&potencia, x + 1);
        liberar_numero(&potencia);
        potencia = siguiente;
    }
    char *texto_resultado = numero_a_texto(&acumulado);
    char *texto_potencia = numero_a_texto(&potencia);
    snprintf(linea, sizeof linea, "f(%u) = ", x);
    imprimir_en_ambos(archivo, "%s", linea);
    imprimir_en_ambos(archivo, "%s\n", texto_resultado);
    snprintf(linea, sizeof linea, "Comprobacion directa (%u+1)^%d = ", x, n);
    imprimir_en_ambos(archivo, "%s", linea);
    imprimir_en_ambos(archivo, "%s", texto_potencia);
    imprimir_en_ambos(archivo, " -> %s\n", strcmp(texto_resultado, texto_potencia) == 0 ? "COINCIDE" : "NO COINCIDE");

    free(texto_resultado); free(texto_potencia);
    liberar_numero(&acumulado);
    liberar_numero(&potencia);
    liberar_triangulo(triangulo, n);
}

static double segundos_transcurridos(struct timespec inicio, struct timespec fin) {
    return (double)(fin.tv_sec - inicio.tv_sec) + (double)(fin.tv_nsec - inicio.tv_nsec) / 1e9;
}

int main(int argc, char *argv[]) {
    /* Trabaja en la carpeta del ejecutable, para encontrar sus archivos desde donde sea */
    char ruta_programa[1024];
    snprintf(ruta_programa, sizeof ruta_programa, "%s", argv[0]);
    if (chdir(dirname(ruta_programa)) != 0) perror("chdir");

    if (argc == 3) {
        int n = atoi(argv[1]);
        int x = atoi(argv[2]);
        if (n < 0 || x < 0) {
            printf("n y x deben ser enteros no negativos\n");
            return 1;
        }
        resolver(n, (uint32_t)x, n <= 10, NULL);
        return 0;
    }

    FILE *archivo = fopen("resultados_c.txt", "w");
    imprimir_en_ambos(archivo, "%s", "=== Ejercicio 2 - Triangulo de Pascal (C) ===\n\n--- Demo n=5, x=2 ---\n");
    resolver(5, 2, 1, archivo);
    imprimir_en_ambos(archivo, "%s", "\n--- n=100, x=2 ---\n");
    resolver(100, 2, 0, archivo);

    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    NumeroGrande **triangulo = generar_triangulo(100);
    clock_gettime(CLOCK_MONOTONIC, &fin);
    liberar_triangulo(triangulo, 100);
    double tiempo_una_vez = segundos_transcurridos(inicio, fin);

    clock_gettime(CLOCK_MONOTONIC, &inicio);
    for (int repeticion = 0; repeticion < REPETICIONES_MEDICION; repeticion++) {
        triangulo = generar_triangulo(100);
        liberar_triangulo(triangulo, 100);
    }
    clock_gettime(CLOCK_MONOTONIC, &fin);
    double tiempo_promedio = segundos_transcurridos(inicio, fin) / REPETICIONES_MEDICION;

    char linea[200];
    snprintf(linea, sizeof linea,
             "\n--- Tiempo de generacion del triangulo para n=100 ---\n"
             "Una ejecucion: %.2f microsegundos\n"
             "Promedio de %d ejecuciones (incluye liberar memoria): %.2f microsegundos\n",
             tiempo_una_vez * 1e6, REPETICIONES_MEDICION, tiempo_promedio * 1e6);
    imprimir_en_ambos(archivo, "%s", linea);
    fclose(archivo);
    return 0;
}
