"""
Ejercicio 2 - Coeficientes de (x+1)^n con el triangulo de Pascal (Python)

a) Construye el triangulo fila por fila: cada valor interior es la suma de los
   dos valores que tiene encima. La fila n son los coeficientes de (x+1)^n.
b) Evalua f(x) = (x+1)^n por pasos con el metodo de Horner:
   acumulado = acumulado * x + coeficiente, desde el grado mas alto.

Memoria dinamica: las filas son listas que crecen en tiempo de ejecucion
(Python reserva y libera esa memoria automaticamente). Los enteros de Python
no tienen limite de tamano, asi que n=100 no desborda.

Uso:
    python3 pascal.py          -> demo n=5, x=2 y mediciones con n=100
    python3 pascal.py 7 3      -> n=7, x=3
"""
import os
import sys
import time

REPETICIONES_MEDICION = 1000


def generar_triangulo(n):
    triangulo = [[1]]
    for numero_fila in range(1, n + 1):
        fila_anterior = triangulo[-1]
        fila_nueva = [1]
        for posicion in range(1, numero_fila):
            fila_nueva.append(fila_anterior[posicion - 1] + fila_anterior[posicion])
        fila_nueva.append(1)
        triangulo.append(fila_nueva)
    return triangulo


def termino_a_texto(coeficiente, exponente):
    if exponente == 0:
        return str(coeficiente)
    parte_coeficiente = "" if coeficiente == 1 else str(coeficiente)
    parte_variable = "x" if exponente == 1 else f"x^{exponente}"
    return parte_coeficiente + parte_variable


def polinomio_a_texto(coeficientes):
    """coeficientes[k] multiplica a x^(n-k)."""
    grado = len(coeficientes) - 1
    terminos = [termino_a_texto(coeficiente, grado - indice)
                for indice, coeficiente in enumerate(coeficientes)]
    return " + ".join(terminos)


def evaluar_por_pasos(coeficientes, x):
    grado = len(coeficientes) - 1
    pasos = []
    acumulado = 0
    for indice, coeficiente in enumerate(coeficientes):
        nuevo_acumulado = acumulado * x + coeficiente
        pasos.append(f"Paso {indice + 1} (x^{grado - indice}): "
                     f"{acumulado} * {x} + {coeficiente} = {nuevo_acumulado}")
        acumulado = nuevo_acumulado
    return acumulado, pasos


def resolver(n, x, mostrar_triangulo):
    lineas = []
    triangulo = generar_triangulo(n)
    if mostrar_triangulo:
        lineas.append("Triangulo de Pascal:")
        for numero_fila, fila in enumerate(triangulo):
            lineas.append(f"  (n={numero_fila}): " + " ".join(map(str, fila)))
    coeficientes = triangulo[n]
    lineas.append(f"Coeficientes de (x+1)^{n}: " + " ".join(map(str, coeficientes)))
    lineas.append(f"(x+1)^{n} = " + polinomio_a_texto(coeficientes))
    lineas.append(f"Evaluacion por pasos (Horner) con x = {x}:")
    resultado, pasos = evaluar_por_pasos(coeficientes, x)
    lineas += ["  " + paso for paso in pasos]
    lineas.append(f"f({x}) = {resultado}")
    lineas.append(f"Comprobacion directa ({x}+1)^{n} = {(x + 1) ** n} -> "
                  + ("COINCIDE" if resultado == (x + 1) ** n else "NO COINCIDE"))
    return lineas


def main():
    # Trabaja en la carpeta del script, para encontrar sus archivos desde donde sea
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    if len(sys.argv) == 3:
        n, x = int(sys.argv[1]), int(sys.argv[2])
        if n < 0 or x < 0:
            print("n y x deben ser enteros no negativos")
            return
        print("\n".join(resolver(n, x, mostrar_triangulo=n <= 10)))
        return

    lineas = ["=== Ejercicio 2 - Triangulo de Pascal (Python) ===", "", "--- Demo n=5, x=2 ---"]
    lineas += resolver(5, 2, mostrar_triangulo=True)
    lineas += ["", "--- n=100, x=2 ---"]
    lineas += resolver(100, 2, mostrar_triangulo=False)

    inicio = time.perf_counter()
    generar_triangulo(100)
    tiempo_una_vez = time.perf_counter() - inicio

    inicio = time.perf_counter()
    for _ in range(REPETICIONES_MEDICION):
        generar_triangulo(100)
    tiempo_promedio = (time.perf_counter() - inicio) / REPETICIONES_MEDICION

    lineas += [
        "",
        "--- Tiempo de generacion del triangulo para n=100 ---",
        f"Una ejecucion: {tiempo_una_vez * 1e6:.2f} microsegundos",
        f"Promedio de {REPETICIONES_MEDICION} ejecuciones: {tiempo_promedio * 1e6:.2f} microsegundos",
    ]

    texto = "\n".join(lineas)
    print(texto)
    with open("resultados_python.txt", "w", encoding="utf-8") as archivo:
        archivo.write(texto + "\n")


if __name__ == "__main__":
    main()
