#include <stdio.h>
#include <stdlib.h>

/* Programa de ejemplo: el while y el int de este comentario NO se traducen */
typedef struct {
    char nombre[50];
    int edad;
    double promedio;
} Estudiante;

enum Estado { ACTIVO, INACTIVO };

static int contador_global = 0;

unsigned long factorial(unsigned int numero) {
    if (numero <= 1) return 1;
    return numero * factorial(numero - 1);
}

void clasificar(int nota) {
    switch (nota / 5) {
        case 4:
            printf("Excelente\n");
            break;
        case 3:
            printf("Aprobado\n");
            break;
        default:
            printf("Reprobado: no use int ni while aqui\n");
    }
}

int main(void) {
    const int limite = 5;
    float suma = 0.0f;
    Estudiante *lista = malloc(sizeof(Estudiante) * limite);

    for (int i = 0; i < limite; i++) {
        if (i % 2 == 0) continue;
        else suma += i;
    }

    int intentos = 3; // el identificador "intentos" contiene "int" pero no es palabra reservada
    do {
        intentos--;
    } while (intentos > 0);

    clasificar(18);
    printf("%lu %f %d\n", factorial(5), suma, contador_global);
    free(lista);
    return 0;
}
