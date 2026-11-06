#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

static const char *RESERVADAS[] = {
    "auto","break","case","char","const","continue","default","do","double",
    "else","enum","extern","float","for","goto","if","inline","int","long",
    "register","restrict","return","short","signed","sizeof","static","struct",
    "switch","typedef","union","unsigned","void","volatile","while", NULL
};

static const char *TRADUCCIONES[] = {
    "automatico","romper","caso","caracter","constante","continuar","defecto","hacer","doble",
    "sino","enumeracion","externo","flotante","para","ir_a","si","en_linea","entero","largo",
    "registro","restringir","retornar","corto","con_signo","tamano_de","estatico","estructura",
    "segun","tipo_def","union","sin_signo","vacio","volatil","mientras"
};

static int buscar(const char *tok) {
    for (int i = 0; RESERVADAS[i]; i++)
        if (strcmp(tok, RESERVADAS[i]) == 0) return i;
    return -1;
}

int main(int argc, char **argv) {
    if (argc < 2) { 
        fprintf(stderr, "Uso: %s <archivo.c>\n", argv[0]); 
        return 1; 
    }
    
    FILE *f = fopen(argv[1], "rb");
    if (!f) { 
        perror("Error al abrir archivo"); 
        return 1; 
    }
    
    fseek(f, 0, SEEK_END); 
    long sz = ftell(f); 
    fseek(f, 0, SEEK_SET);
    
    char *buf = malloc(sz + 1);
    if (!buf) { 
        fclose(f); 
        return 1; 
    }
    
    fread(buf, 1, sz, f); 
    buf[sz] = 0; 
    fclose(f);

    int en_str = 0, en_char = 0, en_lin = 0, en_blk = 0;
    
    double t0 = ahora();
    
    for (long i = 0; i < sz; ) {
        char c = buf[i];
        
        if (en_lin) { 
            putchar(c); 
            if (c == '\n') en_lin = 0; 
            i++; 
            continue; 
        }
        if (en_blk) {
            putchar(c);
            if (c == '*' && i + 1 < sz && buf[i+1] == '/') { 
                putchar('/'); i += 2; en_blk = 0; continue; 
            }
            i++; continue;
        }
        if (en_str) {
            putchar(c);
            if (c == '\\' && i + 1 < sz) { putchar(buf[i+1]); i += 2; continue; }
            if (c == '"') en_str = 0; i++; continue;
        }
        if (en_char) {
            putchar(c);
            if (c == '\\' && i + 1 < sz) { putchar(buf[i+1]); i += 2; continue; }
            if (c == '\'') en_char = 0; i++; continue;
        }
        
        if (c == '/' && i + 1 < sz && buf[i+1] == '/') { en_lin = 1; putchar(c); i++; continue; }
        if (c == '/' && i + 1 < sz && buf[i+1] == '*') { en_blk = 1; putchar('/'); putchar('*'); i += 2; continue; }
        if (c == '"') { en_str = 1; putchar(c); i++; continue; }
        if (c == '\'') { en_char = 1; putchar(c); i++; continue; }
        
        if (isalpha((unsigned char)c) || c == '_') {
            long j = i;
            while (j < sz && (isalnum((unsigned char)buf[j]) || buf[j] == '_')) j++;
            long len = j - i;
            if (len < 64) {
                char tok[64];
                memcpy(tok, buf + i, len); tok[len] = 0;
                int idx = buscar(tok);
                if (idx >= 0) fputs(TRADUCCIONES[idx], stdout);
                else fputs(tok, stdout);
            } else {
                fwrite(buf + i, 1, len, stdout);
            }
            i = j; continue;
        }
        putchar(c); i++;
    }
    
    double t1 = ahora();

    free(buf);
    
    fprintf(stderr, "\n\n--- TIEMPO DE EJECUCION ---\n");
    fprintf(stderr, "Tiempo de busqueda y traduccion: %.9f segundos\n", t1 - t0);
    
    return 0;
}