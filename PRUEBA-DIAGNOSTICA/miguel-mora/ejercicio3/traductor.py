"""
Ejercicio 3 - Traductor de palabras reservadas de C al espanol (Python)

1. Carga el programa C completo en memoria (una cadena creada en ejecucion).
2. Lo recorre caracter por caracter como un analizador lexico sencillo:
   - comentarios (// y /* */), cadenas "..." y caracteres '...' se copian tal cual;
   - cada identificador (letra o _ seguido de letras, digitos o _) se compara
     completo contra la tabla de palabras reservadas.
   Asi "intentos" no se traduce aunque contenga "int".
3. Reemplaza cada palabra reservada por su equivalente en espanol.

Uso:
    python3 traductor.py               -> traduce ejemplo.c y mide tiempos
    python3 traductor.py archivo.c     -> traduce otro archivo
"""
import os
import sys
import time

# Las 32 palabras reservadas del estandar C89/C90
PALABRAS_RESERVADAS = {
    "auto": "automatico", "break": "romper", "case": "caso", "char": "caracter",
    "const": "constante", "continue": "continuar", "default": "por_defecto", "do": "hacer",
    "double": "doble", "else": "sino", "enum": "enumeracion", "extern": "externo",
    "float": "flotante", "for": "para", "goto": "ir_a", "if": "si",
    "int": "entero", "long": "largo", "register": "registro", "return": "retornar",
    "short": "corto", "signed": "con_signo", "sizeof": "tamano_de", "static": "estatico",
    "struct": "estructura", "switch": "segun", "typedef": "definir_tipo", "union": "union",
    "unsigned": "sin_signo", "void": "vacio", "volatile": "volatil", "while": "mientras",
}

REPETICIONES = 100


def es_inicio_identificador(caracter):
    return caracter.isalpha() or caracter == "_"


def es_parte_identificador(caracter):
    return caracter.isalnum() or caracter == "_"


def traducir(codigo):
    """Devuelve (codigo_traducido, palabras_encontradas) con palabra -> cantidad."""
    partes_salida = []
    palabras_encontradas = {}
    posicion = 0
    longitud = len(codigo)

    while posicion < longitud:
        caracter = codigo[posicion]
        siguiente = codigo[posicion + 1] if posicion + 1 < longitud else ""

        if caracter == "/" and siguiente == "/":                    # comentario de linea
            fin = codigo.find("\n", posicion)
            fin = longitud if fin == -1 else fin
            partes_salida.append(codigo[posicion:fin])
            posicion = fin
        elif caracter == "/" and siguiente == "*":                  # comentario de bloque
            fin = codigo.find("*/", posicion + 2)
            fin = longitud if fin == -1 else fin + 2
            partes_salida.append(codigo[posicion:fin])
            posicion = fin
        elif caracter in "\"'":                                     # cadena o caracter
            fin = posicion + 1
            while fin < longitud and codigo[fin] != caracter:
                fin += 2 if codigo[fin] == "\\" else 1
            fin = min(fin + 1, longitud)
            partes_salida.append(codigo[posicion:fin])
            posicion = fin
        elif es_inicio_identificador(caracter):                     # identificador
            fin = posicion + 1
            while fin < longitud and es_parte_identificador(codigo[fin]):
                fin += 1
            palabra = codigo[posicion:fin]
            if palabra in PALABRAS_RESERVADAS:
                palabras_encontradas[palabra] = palabras_encontradas.get(palabra, 0) + 1
                partes_salida.append(PALABRAS_RESERVADAS[palabra])
            else:
                partes_salida.append(palabra)
            posicion = fin
        else:                                                       # cualquier otro simbolo
            partes_salida.append(caracter)
            posicion += 1

    return "".join(partes_salida), palabras_encontradas


def main():
    # Trabaja en la carpeta del script, para encontrar sus archivos desde donde sea
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    ruta_entrada = sys.argv[1] if len(sys.argv) == 2 else "ejemplo.c"
    with open(ruta_entrada, encoding="utf-8") as archivo:
        codigo = archivo.read()

    codigo_traducido, palabras_encontradas = traducir(codigo)

    inicio = time.perf_counter()
    for _ in range(REPETICIONES):
        traducir(codigo)
    tiempo_total = time.perf_counter() - inicio

    lineas = [f"=== Ejercicio 3 - Traductor de palabras reservadas (Python) ===",
              f"Archivo: {ruta_entrada} ({len(codigo)} caracteres cargados en memoria)", "",
              "Palabras reservadas encontradas:"]
    for palabra in sorted(palabras_encontradas):
        lineas.append(f"  {palabra:<10} -> {PALABRAS_RESERVADAS[palabra]:<14} "
                      f"({palabras_encontradas[palabra]} veces)")
    lineas += [f"Total: {sum(palabras_encontradas.values())} apariciones de "
               f"{len(palabras_encontradas)} palabras distintas", "",
               f"Tiempo de traducir el archivo {REPETICIONES} veces (n=100): "
               f"{tiempo_total * 1e6:.2f} microsegundos", "",
               "--- Codigo traducido ---", codigo_traducido]

    texto = "\n".join(lineas)
    print(texto)
    with open("resultados_python.txt", "w", encoding="utf-8") as archivo:
        archivo.write(texto + "\n")


if __name__ == "__main__":
    main()
