/*
 * Archivo de codigo en ingles para el Ejercicio 3.
 * Contiene varias palabras reservadas de C para detectar y traducir.
 */

#include <stdio.h>

int main(void) {
    int numero;
    float promedio;
    char letra;

    numero = 10;
    promedio = 0.0f;
    letra = 'A';

    if (numero > 0) {
        while (numero > 0) {
            promedio = promedio + 1.0f;
            numero = numero - 1;
        }
    } else {
        do {
            promedio = promedio - 1.0f;
        } while (promedio > 0);
    }

    for (numero = 0; numero < 3; numero = numero + 1) {
        switch (numero) {
            case 0:
                break;
            case 1:
                continue;
            default:
                return 0;
        }
    }

    return 0;
}
