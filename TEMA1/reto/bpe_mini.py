"""BPE minimo desde cero (Sennrich et al., 2016): aprende fusiones de pares frecuentes."""
from collections import Counter

def entrenar(texto, n_fusiones):
    palabras = Counter(tuple(p) for p in texto.split())
    fusiones = []
    for _ in range(n_fusiones):
        pares = Counter()
        for w, f in palabras.items():
            for a, b in zip(w, w[1:]):
                pares[(a, b)] += f
        if not pares:
            break
        mejor = max(pares, key=lambda p: (pares[p], p))
        fusiones.append(mejor)
        palabras = _reaplicar(palabras, mejor)
    return fusiones

def _fusionar(w, par):
    out, i = [], 0
    while i < len(w):
        if i < len(w) - 1 and (w[i], w[i + 1]) == par:
            out.append(w[i] + w[i + 1]); i += 2
        else:
            out.append(w[i]); i += 1
    return tuple(out)

def _reaplicar(palabras, par):
    nuevo = Counter()
    for w, f in palabras.items():
        nuevo[_fusionar(w, par)] += f
    return nuevo

def tokenizar(texto, fusiones):
    salida = []
    for p in texto.split():
        w = tuple(p)
        for par in fusiones:
            w = _fusionar(w, par)
        salida.extend(w)
    return salida
