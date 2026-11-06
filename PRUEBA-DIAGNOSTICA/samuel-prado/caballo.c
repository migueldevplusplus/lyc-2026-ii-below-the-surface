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

static int es_salto(int c1, int f1, int c2, int f2) {
    int dc = abs(c2 - c1), df = abs(f2 - f1);
    return (dc == 2 && df == 1) || (dc == 1 && df == 2);
}

static int validar(const char *pos, const char **movs, int n, char *msg) {
    int c = pos[0] - 'a', f = pos[1] - '1';
    for (int i = 0; i < n; i++) {
        if (movs[i][0] != 'K') {
            if (msg) sprintf(msg, "INCORRECTO falla letra %c no pertenece a un caballo", movs[i][0]);
            return 0;
        }
        int nc = movs[i][1] - 'a', nf = movs[i][2] - '1';
        if (nc < 0 || nc > 7 || nf < 0 || nf > 7 || !es_salto(c, f, nc, nf)) {
            if (msg) sprintf(msg, "INCORRECTO falla en el movimiento %d", i + 1);
            return 0;
        }
        c = nc; f = nf;
    }
    if (msg) strcpy(msg, "movimiento CORRECTO");
    return 1;
}

static const char *BIG_MOVES =
"Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 "
"Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 "
"Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
"Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 "
"Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 "
"Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
"Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 "
"Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
"Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 "
"Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 "
"Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
"Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 "
"Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
"Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 "
"Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 "
"Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
"Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 "
"Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
"Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 "
"Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 "
"Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
"Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 "
"Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
"Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 "
"Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 "
"Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 "
"Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 "
"Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 "
"Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 "
"Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 "
"Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 "
"Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 Kb6 Ka4 "
"Kb6 Ka4 Kb6 Ka4";

static int split_moves(const char *s, char tokens[][4], int max) {
    int count = 0;
    const char *p = s;
    while (*p && count < max) {
        while (*p == ' ') p++;
        if (!*p) break;
        int k = 0;
        while (*p && *p != ' ' && k < 3) tokens[count][k++] = *p++;
        tokens[count][k] = 0;
        count++;
    }
    return count;
}

int main(void) {
    char toks[550][4];
    int bn = split_moves(BIG_MOVES, toks, 550);
    const char **mv = malloc(bn * sizeof(char *));
    if (!mv) { fprintf(stderr, "sin memoria\n"); return 1; }
    for (int i = 0; i < bn; i++) mv[i] = toks[i];

    char msg[128];

    validar("a2", mv, bn, msg);
    printf("Validacion previa: %s\n", msg);

    double t0 = ahora();
    for (int r = 0; r < 10; r++) validar("a2", mv, bn, NULL);
    double dt = ahora() - t0;

    printf("Movimientos verificados: %d\n", bn);
    printf("Repeticiones: 10\n");
    printf("Tiempo 10 repeticiones: %.9f s\n", dt);
    printf("Tiempo promedio por verificacion: %.9f s\n", dt / 10.0);

    FILE *out = fopen("resultado.txt", "w");
    if (out) {
        fprintf(out, "Validacion previa: %s\n", msg);
        fprintf(out, "Movimientos verificados: %d\n", bn);
        fprintf(out, "Repeticiones: 10\n");
        fprintf(out, "Tiempo 10 repeticiones: %.9f s\n", dt);
        fprintf(out, "Tiempo promedio por verificacion: %.9f s\n", dt / 10.0);
        fclose(out);
    }
    free(mv);
    return 0;
}