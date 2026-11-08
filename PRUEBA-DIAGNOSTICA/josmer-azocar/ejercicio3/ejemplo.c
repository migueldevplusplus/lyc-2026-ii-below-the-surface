#include <stdio.h>

struct punto {
    int x;
    int y;
};

/* Esta funcion suma los numeros del 1 al limite */
int sumar_hasta(int limite) {
    int suma = 0;
    for (int i = 1; i <= limite; i++) {
        suma += i;
    }
    return suma;
}

int main(void) {
    struct punto p = {3, 4};
    double promedio = 2.5;
    char letra = 'a';
    unsigned int contador = 0;

    while (contador < 3) {
        contador++;
    }

    if (p.x > p.y) {
        printf("x es mayor\n");
    } else {
        printf("y es mayor, el texto int no se traduce\n");
    }

    switch (letra) {
        case 'a':
            printf("letra a\n");
            break;
        default:
            printf("otra letra\n");
    }

    printf("suma = %d, promedio = %f\n", sumar_hasta(5), promedio);
    return 0;
}
