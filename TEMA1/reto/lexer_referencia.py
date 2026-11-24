"""Lexer de referencia: escaner manual (caracter a caracter) con regla de coincidencia mas larga."""
LETRAS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_"
DIGITOS = "0123456789"
RESERVADAS = {"if", "else", "while", "return"}

def lexer(code):
    i, n, out = 0, len(code), []
    while i < n:
        c = code[i]
        if c.isspace():
            i += 1
        elif code.startswith("//", i):
            j = code.find("\n", i)
            j = n if j == -1 else j
            out.append(("COMENTARIO", code[i:j])); i = j
        elif c in LETRAS:
            j = i
            while j < n and (code[j] in LETRAS or code[j] in DIGITOS):
                j += 1
            t = code[i:j]
            out.append(("PALABRA_RESERVADA" if t in RESERVADAS else "IDENTIFICADOR", t)); i = j
        elif c in DIGITOS:
            j = i
            while j < n and code[j] in DIGITOS:
                j += 1
            if j + 1 < n and code[j] == "." and code[j + 1] in DIGITOS:
                j += 1
                while j < n and code[j] in DIGITOS:
                    j += 1
            out.append(("NUMERO", code[i:j])); i = j
        elif c == '"':
            j = i + 1
            while j < n and code[j] not in '"\n':
                j += 1
            if j < n and code[j] == '"':
                out.append(("CADENA", code[i:j + 1])); i = j + 1
            else:
                out.append(("ERROR", c)); i += 1
        elif code[i:i + 2] in ("==", "!=", "<=", ">="):
            out.append(("OPERADOR", code[i:i + 2])); i += 2
        elif c in "+-*/=<>":
            out.append(("OPERADOR", c)); i += 1
        elif c in "(){};,":
            out.append(("DELIMITADOR", c)); i += 1
        else:
            out.append(("ERROR", c)); i += 1
    return out
