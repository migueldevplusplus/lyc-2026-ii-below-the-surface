"""Lexer generado por un LLM (Claude) a partir de la especificacion del mini-lenguaje (ver prompt.md)."""
import re

RESERVADAS = {"if", "else", "while", "return"}
ESPECIFICACION = [
    ("COMENTARIO", r"//[^\n]*"),
    ("CADENA", r'"[^"\n]*"'),
    ("NUMERO", r"[0-9]+(?:\.[0-9]+)?"),
    ("IDENTIFICADOR", r"[A-Za-z_][A-Za-z0-9_]*"),
    ("OPERADOR", r"==|!=|<=|>=|[+\-*/=<>]"),
    ("DELIMITADOR", r"[(){};,]"),
    ("ESPACIO", r"\s+"),
    ("ERROR", r"."),
]
PATRON = re.compile("|".join(f"(?P<{n}>{p})" for n, p in ESPECIFICACION))

def lexer(code):
    out = []
    for m in PATRON.finditer(code):
        tipo, texto = m.lastgroup, m.group()
        if tipo == "ESPACIO":
            continue
        if tipo == "IDENTIFICADOR" and texto in RESERVADAS:
            tipo = "PALABRA_RESERVADA"
        out.append((tipo, texto))
    return out
