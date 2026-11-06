import sys
import time

RESERVADAS = {
    "auto":"automatico","break":"romper","case":"caso","char":"caracter","const":"constante",
    "continue":"continuar","default":"defecto","do":"hacer","double":"doble","else":"sino",
    "enum":"enumeracion","extern":"externo","float":"flotante","for":"para","goto":"ir_a",
    "if":"si","inline":"en_linea","int":"entero","long":"largo","register":"registro",
    "restrict":"restringir","return":"retornar","short":"corto","signed":"con_signo",
    "sizeof":"tamano_de","static":"estatico","struct":"estructura","switch":"segun",
    "typedef":"tipo_def","union":"union","unsigned":"sin_signo","void":"vacio",
    "volatile":"volatil","while":"mientras"
}

def traducir(texto):
    out = []
    i, n = 0, len(texto)
    
    t0 = time.perf_counter()
    
    while i < n:
        c = texto[i]
        
        if c == '/' and i + 1 < n and texto[i+1] == '/':
            j = texto.find('\n', i)
            if j == -1:
                out.append(texto[i:]); break
            out.append(texto[i:j+1]); i = j + 1; continue
            
        if c == '/' and i + 1 < n and texto[i+1] == '*':
            j = texto.find('*/', i + 2)
            if j == -1:
                out.append(texto[i:]); break
            out.append(texto[i:j+2]); i = j + 2; continue
            
        if c == '"' or c == "'":
            quote = c; j = i + 1
            while j < n:
                if texto[j] == '\\' and j + 1 < n:
                    j += 2; continue
                if texto[j] == quote:
                    j += 1; break
                j += 1
            out.append(texto[i:j]); i = j; continue
            
        if c.isalpha() or c == '_':
            j = i
            while j < n and (texto[j].isalnum() or texto[j] == '_'):
                j += 1
            tok = texto[i:j]
            out.append(RESERVADAS.get(tok, tok))
            i = j; continue
            
        out.append(c); i += 1
        
    t1 = time.perf_counter()
    
    print("\n\n--- TIEMPO DE EJECUCION ---", file=sys.stderr)
    print(f"Tiempo de busqueda y traduccion: {t1 - t0:.9f} segundos", file=sys.stderr)
    
    return "".join(out)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print(f"Uso: {sys.argv[0]} <archivo.c>", file=sys.stderr)
        sys.exit(1)
        
    with open(sys.argv[1], "r", encoding="utf-8", errors="replace") as f:
        texto = f.read()
        
    sys.stdout.write(traducir(texto))