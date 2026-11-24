"""Ejecuta todo el reto:  python main.py"""
from lexer_referencia import lexer as ref
from lexer_llm import lexer as llm
import bpe_mini

PROGRAMAS = [
    'if (x1 >= 10) return x1;',
    'while (i <= 3.14) { i = i + 1; } // fin',
    'a == b != c',
    'ifx if_ else1 return',
    's = "hola // no es comentario";',
    '3. 1.5.2 .5',
    'x = "sin cerrar;',
    'y = a @ b ! c',
    'f(a, b); // coma\nz = 2*3-1/4;',
]

print("=== PRUEBAS DIFERENCIALES (lexer de referencia vs lexer del LLM) ===")
fallos = 0
for p in PROGRAMAS:
    a, b = ref(p), llm(p)
    if a != b:
        fallos += 1
        print(f"DISCREPANCIA en: {p!r}\n  referencia: {a}\n  llm:        {b}")
print(f"{len(PROGRAMAS)} programas probados, {fallos} discrepancias.\n")

print("=== COMPARACION CON BPE ===")
corpus = "\n".join(PROGRAMAS)
fus = bpe_mini.entrenar(corpus, 20)
for p in PROGRAMAS[:3]:
    print("Codigo:   ", p)
    print("Lexer:    ", [t for _, t in ref(p)])
    print("BPE (20): ", bpe_mini.tokenizar(p, fus))
    print()
