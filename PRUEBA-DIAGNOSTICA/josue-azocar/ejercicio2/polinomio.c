/*
 * Ejercicio 2 - Modulo en C: coeficientes de (x+1)^n (triangulo de Pascal).
 * Memoria dinamica (malloc/free) + enteros grandes.
 *
 * Por que enteros grandes:
 *   C(100, 50) supera long/long long; sin ellos n=100 desbordaria.
 *
 * Uso (desde polinomio.py):
 *   polinomio.exe calcular <n> <x>
 *   polinomio.exe tiempo
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BASE 1000000000
#define REPETICIONES_TIEMPO 30
#define ANCHO 72

/* Entero no negativo de precision arbitraria (digitos en base 1e9). */
typedef struct {
    int *digitos;
    int cantidad;
} EnteroGrande;

static void imprimir_linea(char c) {
    int i;
    for (i = 0; i < ANCHO; i++) putchar(c);
    putchar('\n');
}

static EnteroGrande *eg_crear_desde_int(int valor) {
    EnteroGrande *n = (EnteroGrande *)malloc(sizeof(EnteroGrande));
    if (!n) return NULL;
    n->digitos = (int *)malloc(sizeof(int));
    if (!n->digitos) {
        free(n);
        return NULL;
    }
    if (valor < 0) valor = 0;
    n->digitos[0] = valor % BASE;
    n->cantidad = 1;
    if (valor >= BASE) {
        int *nuevo = (int *)realloc(n->digitos, 2 * sizeof(int));
        if (!nuevo) {
            free(n->digitos);
            free(n);
            return NULL;
        }
        n->digitos = nuevo;
        n->digitos[1] = valor / BASE;
        n->cantidad = 2;
    }
    return n;
}

static void eg_liberar(EnteroGrande *n) {
    if (!n) return;
    free(n->digitos);
    free(n);
}

static int eg_sumar_en_sitio(EnteroGrande *a, const EnteroGrande *b) {
    int max = a->cantidad > b->cantidad ? a->cantidad : b->cantidad;
    int *nuevo = (int *)realloc(a->digitos, (size_t)(max + 1) * sizeof(int));
    long long acarreo = 0;
    int i;

    if (!nuevo) return 0;
    a->digitos = nuevo;

    for (i = 0; i < max; i++) {
        long long da = (i < a->cantidad) ? a->digitos[i] : 0;
        long long db = (i < b->cantidad) ? b->digitos[i] : 0;
        long long suma = da + db + acarreo;
        a->digitos[i] = (int)(suma % BASE);
        acarreo = suma / BASE;
    }
    a->cantidad = max;
    if (acarreo) a->digitos[a->cantidad++] = (int)acarreo;
    return 1;
}

static void eg_imprimir(const EnteroGrande *n) {
    int i;
    printf("%d", n->digitos[n->cantidad - 1]);
    for (i = n->cantidad - 2; i >= 0; i--) printf("%09d", n->digitos[i]);
}

static int eg_es_cero(const EnteroGrande *n) {
    return n->cantidad == 1 && n->digitos[0] == 0;
}

static int eg_es_uno(const EnteroGrande *n) {
    return n->cantidad == 1 && n->digitos[0] == 1;
}

static long eg_a_long(const EnteroGrande *n, int *ok) {
    long r = 0;
    int i;
    for (i = n->cantidad - 1; i >= 0; i--) {
        if (r > (2147483647L - n->digitos[i]) / BASE) {
            *ok = 0;
            return 0;
        }
        r = r * BASE + n->digitos[i];
    }
    *ok = 1;
    return r;
}

static long potencia_long(long base, int grado) {
    long r = 1;
    int i;
    for (i = 0; i < grado; i++) r *= base;
    return r;
}

static void liberar_fila(EnteroGrande **fila, int cantidad) {
    int i;
    if (!fila) return;
    for (i = 0; i < cantidad; i++) eg_liberar(fila[i]);
    free(fila);
}

/*
 * Construye la fila n in-place (de atras hacia adelante).
 * Misma idea del triangulo de Pascal, con menos memoria temporal.
 */
static EnteroGrande **generar_coeficientes(int n) {
    EnteroGrande **fila;
    int i, j;

    fila = (EnteroGrande **)malloc((size_t)(n + 1) * sizeof(EnteroGrande *));
    if (!fila) return NULL;

    fila[0] = eg_crear_desde_int(1);
    if (!fila[0]) {
        free(fila);
        return NULL;
    }

    for (i = 1; i <= n; i++) {
        fila[i] = eg_crear_desde_int(1);
        if (!fila[i]) {
            liberar_fila(fila, i);
            return NULL;
        }
        for (j = i - 1; j >= 1; j--) {
            if (!eg_sumar_en_sitio(fila[j], fila[j - 1])) {
                liberar_fila(fila, i + 1);
                return NULL;
            }
        }
    }
    return fila;
}

static void imprimir_termino(const EnteroGrande *coef, int grado) {
    if (eg_es_cero(coef)) return;
    if (grado == 0) {
        eg_imprimir(coef);
        return;
    }
    if (!eg_es_uno(coef)) eg_imprimir(coef);
    if (grado == 1) printf("x");
    else printf("x^%d", grado);
}

static void imprimir_polinomio(EnteroGrande **coef, int n) {
    int grado;
    int primero = 1;

    for (grado = n; grado >= 0; grado--) {
        if (eg_es_cero(coef[grado])) continue;
        printf(primero ? "  " : "+ ");
        imprimir_termino(coef[grado], grado);
        printf("\n");
        primero = 0;
    }
    if (primero) printf("  0\n");
}

static void evaluar_paso_a_paso(EnteroGrande **coef, int n, long x) {
    long total = 0;
    int grado, ok, paso = 1;

    printf("\n");
    imprimir_linea('=');
    printf("  CALCULO PASO A PASO  |  f(%ld) = (x + 1)^%d  [C]\n", x, n);
    imprimir_linea('=');
    printf("%-6s%-7s%-32s%12s%12s\n", "Paso", "Grado", "Expresion", "Termino", "Suma");
    imprimir_linea('-');

    for (grado = n; grado >= 0; grado--) {
        long c = eg_a_long(coef[grado], &ok);
        long pot, termino;
        char expresion[64];

        if (!ok) {
            printf("  (coeficiente demasiado grande para evaluar en C con long)\n");
            printf("  Use la opcion Python para n grandes.\n");
            return;
        }

        pot = potencia_long(x, grado);
        termino = c * pot;
        total += termino;

        if (grado == 0) sprintf(expresion, "%ld", c);
        else if (grado == 1) sprintf(expresion, "%ld * %ld", c, x);
        else sprintf(expresion, "%ld * %ld^%d = %ld * %ld", c, x, grado, c, pot);

        if ((int)strlen(expresion) > 30) {
            strcpy(expresion + 27, "...");
        }

        printf("%-6d%-7d%-32s%12ld%12ld\n", paso, grado, expresion, termino, total);
        paso++;
    }

    imprimir_linea('-');
    printf("  Resultado:  f(%ld) = %ld\n", x, total);
    printf("  Comprobacion: (%ld + 1)^%d = %ld\n", x, n, potencia_long(x + 1, n));
    imprimir_linea('=');
}

static int modo_calcular(int n, long x) {
    EnteroGrande **coef;
    int i;

    if (n < 0) {
        printf("n invalido.\n");
        return 1;
    }

    coef = generar_coeficientes(n);
    if (!coef) {
        printf("Error: no hay memoria suficiente.\n");
        return 1;
    }

    printf("\nLenguaje: C\n");
    printf("Coeficientes (fila %d de Pascal):\n  ", n);
    for (i = 0; i <= n; i++) {
        eg_imprimir(coef[i]);
        if (i < n) printf("  ");
    }
    printf("\n\n(x+1)^%d =\n", n);
    imprimir_polinomio(coef, n);
    evaluar_paso_a_paso(coef, n, x);

    liberar_fila(coef, n + 1);
    return 0;
}

static int modo_tiempo(void) {
    const int n = 100;
    EnteroGrande **coef = NULL;
    clock_t inicio, fin;
    int r;

    inicio = clock();
    for (r = 0; r < REPETICIONES_TIEMPO; r++) {
        if (coef) liberar_fila(coef, n + 1);
        coef = generar_coeficientes(n);
        if (!coef) {
            printf("Error: no hay memoria suficiente.\n");
            return 1;
        }
    }
    fin = clock();

    printf("TIEMPO_C=%.6f\n",
           ((double)(fin - inicio) / CLOCKS_PER_SEC) / (double)REPETICIONES_TIEMPO);
    printf("\nPolinomio (x+1)^%d [generado en C] =\n", n);
    imprimir_polinomio(coef, n);

    liberar_fila(coef, n + 1);
    return 0;
}

static void mostrar_ayuda(const char *nombre) {
    printf("Modulo C del Ejercicio 2. Uso:\n");
    printf("  %s calcular <n> <x>\n", nombre);
    printf("  %s tiempo\n", nombre);
}

int main(int argc, char *argv[]) {
    if (argc >= 2 && strcmp(argv[1], "calcular") == 0 && argc == 4) {
        return modo_calcular(atoi(argv[2]), atol(argv[3]));
    }
    if (argc >= 2 && strcmp(argv[1], "tiempo") == 0) {
        return modo_tiempo();
    }
    mostrar_ayuda(argc > 0 ? argv[0] : "polinomio.exe");
    return 1;
}
