#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TOTAL_RESERVADAS 32

/* Las 32 palabras reservadas de C y su traduccion */
const char *reservadas[TOTAL_RESERVADAS] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

const char *traducciones[TOTAL_RESERVADAS] = {
    "automatico", "romper", "caso", "caracter", "constante", "continuar", "por_defecto", "hacer",
    "doble", "sino", "enumeracion", "externo", "flotante", "para", "ir_a", "si",
    "entero", "largo", "registro", "retornar", "corto", "con_signo", "tamano_de", "estatico",
    "estructura", "segun", "definir_tipo", "union", "sin_signo", "vacio", "volatil", "mientras"
};

char *cargar_archivo(const char *nombre) {
    FILE *archivo = fopen(nombre, "r");
    if (archivo == NULL) {
        return NULL;
    }

    fseek(archivo, 0, SEEK_END);
    long tam = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);

    char *codigo = malloc(tam + 1);
    long leidos = fread(codigo, 1, tam, archivo);
    codigo[leidos] = '\0';

    fclose(archivo);
    return codigo;
}

/* Devuelve la posicion en la lista, o -1 si no es reservada */
int buscar_reservada(const char *palabra, int largo) {
    for (int i = 0; i < TOTAL_RESERVADAS; i++) {
        if ((int)strlen(reservadas[i]) == largo && strncmp(reservadas[i], palabra, largo) == 0) {
            return i;
        }
    }
    return -1;
}

/* Se reserva el triple porque ninguna traduccion mide mas del triple (enum -> enumeracion) */
char *traducir(const char *codigo, int conteo[]) {
    char *traducido = malloc(strlen(codigo) * 3 + 1);
    int i = 0;
    int j = 0;

    for (int k = 0; k < TOTAL_RESERVADAS; k++) {
        conteo[k] = 0;
    }

    while (codigo[i] != '\0') {
        /* Los comentarios y los textos entre comillas se copian sin traducir */
        if (codigo[i] == '/' && codigo[i + 1] == '/') {
            while (codigo[i] != '\0' && codigo[i] != '\n') {
                traducido[j] = codigo[i];
                i++;
                j++;
            }
        } else if (codigo[i] == '/' && codigo[i + 1] == '*') {
            traducido[j] = codigo[i];
            i++;
            j++;
            traducido[j] = codigo[i];
            i++;
            j++;
            while (codigo[i] != '\0' && !(codigo[i] == '*' && codigo[i + 1] == '/')) {
                traducido[j] = codigo[i];
                i++;
                j++;
            }
            if (codigo[i] != '\0') {
                traducido[j] = codigo[i];
                i++;
                j++;
                traducido[j] = codigo[i];
                i++;
                j++;
            }
        } else if (codigo[i] == '"' || codigo[i] == '\'') {
            char comilla = codigo[i];
            traducido[j] = codigo[i];
            i++;
            j++;
            while (codigo[i] != '\0' && codigo[i] != comilla) {
                if (codigo[i] == '\\' && codigo[i + 1] != '\0') {
                    traducido[j] = codigo[i];
                    i++;
                    j++;
                }
                traducido[j] = codigo[i];
                i++;
                j++;
            }
            if (codigo[i] != '\0') {
                traducido[j] = codigo[i];
                i++;
                j++;
            }
        } else if (isalpha(codigo[i]) || codigo[i] == '_') {
            int inicio = i;
            while (isalnum(codigo[i]) || codigo[i] == '_') {
                i++;
            }
            int largo = i - inicio;

            int pos = buscar_reservada(&codigo[inicio], largo);
            if (pos != -1) {
                strcpy(&traducido[j], traducciones[pos]);
                j += strlen(traducciones[pos]);
                conteo[pos]++;
            } else {
                memcpy(&traducido[j], &codigo[inicio], largo);
                j += largo;
            }
        } else {
            traducido[j] = codigo[i];
            i++;
            j++;
        }
    }

    traducido[j] = '\0';
    return traducido;
}

void escribir_resultado(FILE *salida, int conteo[], const char *traducido) {
    int encontradas = 0;

    fprintf(salida, "=== Palabras reservadas encontradas ===\n");
    for (int k = 0; k < TOTAL_RESERVADAS; k++) {
        if (conteo[k] > 0) {
            fprintf(salida, "  %-10s -> %-14s (%d veces)\n", reservadas[k], traducciones[k], conteo[k]);
            encontradas++;
        }
    }
    if (encontradas == 0) {
        fprintf(salida, "  No se encontraron palabras reservadas\n");
    }

    fprintf(salida, "\n=== Codigo traducido ===\n");
    fputs(traducido, salida);
}

int main(void) {
    char nombre[200];
    int conteo[TOTAL_RESERVADAS];

    printf("Archivo C a traducir (Enter para usar ejemplo.c): ");
    if (fgets(nombre, sizeof(nombre), stdin) == NULL) {
        return 0;
    }
    int largo = strlen(nombre);
    if (largo > 0 && nombre[largo - 1] == '\n') {
        nombre[largo - 1] = '\0';
    }
    if (strlen(nombre) == 0) {
        strcpy(nombre, "ejemplo.c");
    }

    char *codigo = cargar_archivo(nombre);
    if (codigo == NULL) {
        printf("No se pudo abrir el archivo %s\n", nombre);
        return 1;
    }

    char *traducido = traducir(codigo, conteo);

    printf("\n");
    escribir_resultado(stdout, conteo, traducido);

    FILE *archivo = fopen("traducido.txt", "w");
    escribir_resultado(archivo, conteo, traducido);
    fclose(archivo);
    printf("\nTraduccion guardada en traducido.txt\n");

    free(codigo);
    free(traducido);
    return 0;
}
