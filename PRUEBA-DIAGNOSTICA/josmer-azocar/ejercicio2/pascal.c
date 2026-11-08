#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/* __int128 guarda exactos los coeficientes hasta n=127 */
#define N_MAXIMO 127

void imprimir_grande(FILE *salida, __int128 num) {
    char digitos[50];
    int i = 0;

    if (num == 0) {
        fputc('0', salida);
        return;
    }
    while (num > 0) {
        digitos[i] = '0' + (int)(num % 10);
        num = num / 10;
        i++;
    }
    while (i > 0) {
        i--;
        fputc(digitos[i], salida);
    }
}

/* Cada fila nueva se reserva con malloc y la anterior se libera */
__int128 *generar_coefs(int n) {
    __int128 *fila = malloc(sizeof(__int128));
    fila[0] = 1;

    for (int i = 1; i <= n; i++) {
        __int128 *nueva = malloc((i + 1) * sizeof(__int128));
        nueva[0] = 1;
        for (int j = 1; j < i; j++) {
            nueva[j] = fila[j - 1] + fila[j];
        }
        nueva[i] = 1;
        free(fila);
        fila = nueva;
    }
    return fila;
}

void imprimir_polinomio(FILE *salida, __int128 *coefs, int n) {
    for (int k = 0; k <= n; k++) {
        int exp = n - k;
        if (k > 0) {
            fputc('+', salida);
        }
        if (coefs[k] != 1 || exp == 0) {
            imprimir_grande(salida, coefs[k]);
        }
        if (exp >= 1) {
            fputc('x', salida);
        }
        if (exp >= 2) {
            fprintf(salida, "^%d", exp);
        }
    }
    fputc('\n', salida);
}

/* double porque (x+1)^n crece muy rapido; si es muy grande el valor es aproximado */
void evaluar_por_pasos(__int128 *coefs, int n, long x) {
    double acumulado = 0;

    for (int k = 0; k <= n; k++) {
        int exp = n - k;
        double termino = (double)coefs[k] * pow(x, exp);
        acumulado += termino;
        printf("  Paso %d : ", k + 1);
        imprimir_grande(stdout, coefs[k]);
        printf(" * %ld^%d = %.0f   acumulado = %.0f\n", x, exp, termino, acumulado);
    }
    printf("  f(%ld) = %.0f\n", x, acumulado);
    printf("  Comprobacion: (%ld+1)^%d = %.0f\n", x, n, pow(x + 1, n));
}

int leer_entero(const char *mensaje, long *valor) {
    char linea[100];

    printf("%s", mensaje);
    if (fgets(linea, sizeof(linea), stdin) == NULL) {
        exit(0);
    }
    return sscanf(linea, "%ld", valor) == 1;
}

void generar_y_evaluar(void) {
    long n, x;

    if (!leer_entero("n (entero entre 0 y 127): ", &n) || n < 0 || n > N_MAXIMO) {
        printf("  n invalido\n");
        return;
    }

    __int128 *coefs = generar_coefs(n);
    printf("  Coeficientes:");
    for (int k = 0; k <= n; k++) {
        printf(" ");
        imprimir_grande(stdout, coefs[k]);
    }
    printf("\n  (x+1)^%ld = ", n);
    imprimir_polinomio(stdout, coefs, n);

    if (!leer_entero("x (entero): ", &x)) {
        printf("  x invalido\n");
    } else {
        evaluar_por_pasos(coefs, n, x);
    }
    free(coefs);
}

double tiempo_actual_ms(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000.0 + t.tv_nsec / 1000000.0;
}

void escribir_resultados(FILE *salida, double tiempo_ms, __int128 *coefs, int n) {
    fprintf(salida, "Lenguaje: C\n");
    fprintf(salida, "n = %d\n", n);
    fprintf(salida, "Tiempo de generacion: %.4f ms\n", tiempo_ms);
    fprintf(salida, "\n(x+1)^%d = ", n);
    imprimir_polinomio(salida, coefs, n);
}

void prueba_de_tiempo(void) {
    int n = 100;

    double inicio = tiempo_actual_ms();
    __int128 *coefs = generar_coefs(n);
    double fin = tiempo_actual_ms();
    double tiempo_ms = fin - inicio;

    escribir_resultados(stdout, tiempo_ms, coefs, n);

    FILE *archivo = fopen("resultados_c.txt", "w");
    escribir_resultados(archivo, tiempo_ms, coefs, n);
    fclose(archivo);
    printf("\n  Resultados guardados en resultados_c.txt\n");

    free(coefs);
}

int main(void) {
    long opcion;

    while (1) {
        printf("\n1. Generar polinomio (x+1)^n y calcular f(x)\n");
        printf("2. Medir tiempo de generar el polinomio para n=100\n");
        printf("0. Salir\n");

        if (!leer_entero("Opcion: ", &opcion)) {
            printf("  Opcion invalida\n");
        } else if (opcion == 1) {
            generar_y_evaluar();
        } else if (opcion == 2) {
            prueba_de_tiempo();
        } else if (opcion == 0) {
            break;
        } else {
            printf("  Opcion invalida\n");
        }
    }
    return 0;
}
