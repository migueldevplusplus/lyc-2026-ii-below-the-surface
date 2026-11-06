#include <stdio.h>

// Este es un comentario con int y while que NO se debe traducir
/* Comentario de bloque
   con for y return */

int main() {
    int x = 10;
    float y = 2.5;
    char *msg = "if else int float";
    
    if (x > 0) {
        while (x > 0) {
            x = x - 1;
        }
    } else {
        return 1;
    }
    
    printf("%s %d\n", msg, x);
    return 0;
}