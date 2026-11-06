#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
static double ahora(void) {
    static LARGE_INTEGER freq;
    static int init = 0;
    LARGE_INTEGER t;
    if (!init) { QueryPerformanceFrequency(&freq); init = 1; }
    QueryPerformanceCounter(&t);
    return (double)t.QuadPart / (double)freq.QuadPart;
}
#else
static double ahora(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec / 1e9;
}
#endif

static unsigned __int128 *generar(int n) {
    unsigned __int128 *c = malloc((n + 1) * sizeof(unsigned __int128));
    if (!c) return NULL;
    for (int i = 0; i <= n; i++) c[i] = 0;
    c[0] = 1;
    for (int i = 1; i <= n; i++)
        for (int j = i; j >= 1; j--)
            c[j] += c[j - 1];
    return c;
}

static void print_u128(unsigned __int128 n) {
    if (n == 0) { putchar('0'); return; }
    char buf[50]; int i = 0;
    while (n > 0) { buf[i++] = '0' + (int)(n % 10); n /= 10; }
    while (i > 0) putchar(buf[--i]);
}

// Convertimos a double (64 bits) para evitar el bug de MinGW con long double
static double u128_to_double(unsigned __int128 n) {
    if (n == 0) return 0.0;
    char buf[50]; int i = 0;
    unsigned __int128 temp = n;
    while (temp > 0) { buf[i++] = '0' + (int)(temp % 10); temp /= 10; }
    char str[51];
    for (int j = 0; j < i; j++) str[j] = buf[i - 1 - j];
    str[i] = '\0';
    return strtod(str, NULL);
}

static void mostrar_polinomio(unsigned __int128 *c, int n) {
    printf("(x+1)^%d = ", n);
    int primero = 1;
    for (int k = 0; k <= n; k++) {
        int pot = n - k;
        if (c[k] == 0) continue;
        if (!primero) printf(" + ");
        primero = 0;
        if (c[k] != 1 || pot == 0) print_u128(c[k]);
        if (pot >= 1) {
            if (c[k] != 1) printf("*");
            printf("x");
            if (pot > 1) printf("^%d", pot);
        }
    }
    printf("\nCoeficientes: ");
    for (int k = 0; k <= n; k++) { print_u128(c[k]); printf(" "); }
    printf("\n");
}

static void paso_a_paso(unsigned __int128 *c, int n, int x) {
    printf("\nPaso a paso f(%d) = (%d + 1)^%d:\n", x, x, n);
    double suma = 0;
    for (int k = 0; k <= n; k++) {
        int pot = n - k;
        double coef = u128_to_double(c[k]);
        double factor = 1.0;
        for (int p = 0; p < pot; p++) factor *= (double)x;
        double term = coef * factor;
        
        // Usamos %e estándar para double
        printf("  term[%d] = %d^%d * coef[%d] = %e\n", k, x, pot, k, term);
        suma += term;
    }
    printf("  f(%d) = %.10e\n", x, suma);
}

static double medir(int n, int reps) {
    double t0 = ahora();
    for (int r = 0; r < reps; r++) {
        unsigned __int128 *c = generar(n);
        free(c);
    }
    return ahora() - t0;
}

int main(int argc, char **argv) {
    int n = (argc >= 2) ? atoi(argv[1]) : 5;
    int x = (argc >= 3) ? atoi(argv[2]) : 2;

    if (n < 0 || n > 130) {
        fprintf(stderr, "n fuera de rango (0..130)\n");
        return 1;
    }

    unsigned __int128 *c = generar(n);
    if (!c) { fprintf(stderr, "sin memoria\n"); return 1; }
    mostrar_polinomio(c, n);
    paso_a_paso(c, n, x);
    free(c);

    double dt = medir(100, 10);
    printf("\n--- Tiempo ---\n");
    printf("Lenguaje: C\n");
    printf("n = 100, repeticiones = 10\n");
    printf("Tiempo total: %.9f s\n", dt);
    printf("Tiempo promedio: %.9f s\n", dt / 10.0);

    FILE *f = fopen("resultado_c.txt", "w");
    if (f) {
        fprintf(f, "Lenguaje: C\n");
        fprintf(f, "n = 100, repeticiones = 10\n");
        fprintf(f, "Tiempo total: %.9f s\n", dt);
        fprintf(f, "Tiempo promedio: %.9f s\n", dt / 10.0);
        fclose(f);
    }
    return 0;
}
