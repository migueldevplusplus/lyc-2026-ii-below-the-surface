/* Copia traducida generada por el Ejercicio 3 */
/* Palabras reservadas de C sustituidas por su equivalente en espanol */

/*
 * Archivo de codigo en ingles para el Ejercicio 3.
 * Contiene varias palabras reservadas de C para detectar y traducir.
 */

#include <stdio.h>

entero main(vacio) {
    entero numero;
    flotante promedio;
    caracter letra;

    numero = 10;
    promedio = 0.0f;
    letra = 'A';

    si (numero > 0) {
        mientras (numero > 0) {
            promedio = promedio + 1.0f;
            numero = numero - 1;
        }
    } si_no {
        hacer {
            promedio = promedio - 1.0f;
        } mientras (promedio > 0);
    }

    para (numero = 0; numero < 3; numero = numero + 1) {
        segun (numero) {
            caso 0:
                romper;
            caso 1:
                continuar;
            por_defecto:
                retornar 0;
        }
    }

    retornar 0;
}
