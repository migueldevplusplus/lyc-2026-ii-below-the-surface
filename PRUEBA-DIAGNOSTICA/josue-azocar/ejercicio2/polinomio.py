"""
Ejercicio 2 - Programa principal: polinomio (x+1)^n con triángulo de Pascal.

Un solo programa (Python + módulo C):
  - genera coeficientes, muestra el polinomio y el cálculo paso a paso
  - compara tiempos Python vs C para n=100

Al finalizar cada ejecución útil, guarda la salida en resultado.txt
dentro de esta misma carpeta.
"""

import os
import subprocess
import sys
import time

CARPETA = os.path.dirname(os.path.abspath(__file__))
EJECUTABLE_C = os.path.join(CARPETA, "polinomio.exe")
ARCHIVO_RESULTADO = "resultado.txt"
REPETICIONES_TIEMPO = 30  # igual que en polinomio.c
ANCHO = 72


class TeeSalida:
    """Escribe a la vez en consola y en un archivo."""

    def __init__(self, *destinos):
        self.destinos = destinos

    def write(self, texto):
        for destino in self.destinos:
            destino.write(texto)
            destino.flush()

    def flush(self):
        for destino in self.destinos:
            destino.flush()


def ruta_resultado():
    return os.path.join(CARPETA, ARCHIVO_RESULTADO)


def con_resultado_en_archivo(funcion):
    """Ejecuta funcion() duplicando la salida hacia resultado.txt."""
    ruta = ruta_resultado()
    with open(ruta, "w", encoding="utf-8") as archivo:
        anterior = sys.stdout
        sys.stdout = TeeSalida(anterior, archivo)
        try:
            funcion()
        finally:
            sys.stdout = anterior
    print(f"\nResultado guardado en: {ruta}")


def generar_coeficientes(n):
    """
    Fila n del triángulo de Pascal (listas dinámicas).
    Actualización in-place: de atrás hacia adelante.
    En C se usan enteros grandes porque C(100, 50) no cabe en long.
    """
    fila = [1]
    for i in range(1, n + 1):
        fila.append(1)
        for j in range(i - 1, 0, -1):
            fila[j] += fila[j - 1]
    return fila


def formatear_termino(coeficiente, grado):
    """Texto de un término; None si el coeficiente es 0."""
    if coeficiente == 0:
        return None
    if grado == 0:
        return str(coeficiente)
    parte_coef = "" if coeficiente == 1 else str(coeficiente)
    parte_x = "x" if grado == 1 else f"x^{grado}"
    return parte_coef + parte_x


def imprimir_polinomio_ordenado(coeficientes):
    """Imprime el polinomio con un término por línea."""
    n = len(coeficientes) - 1
    primero = True
    for grado in range(n, -1, -1):
        texto = formatear_termino(coeficientes[grado], grado)
        if texto is None:
            continue
        print(("  " if primero else "+ ") + texto)
        primero = False
    if primero:
        print("  0")


def evaluar_paso_a_paso(coeficientes, x, etiqueta="Python"):
    """Tabla del cálculo de f(x) término a término."""
    n = len(coeficientes) - 1
    total = 0

    print()
    print("=" * ANCHO)
    print(f"  CALCULO PASO A PASO  |  f({x}) = (x + 1)^{n}  [{etiqueta}]")
    print("=" * ANCHO)
    print(f"{'Paso':^6}{'Grado':^7}{'Expresion':<32}{'Termino':>12}{'Suma':>12}")
    print("-" * ANCHO)

    for paso, grado in enumerate(range(n, -1, -1), start=1):
        coef = coeficientes[grado]
        potencia = x ** grado
        termino = coef * potencia
        total += termino

        if grado == 0:
            expresion = f"{coef}"
        elif grado == 1:
            expresion = f"{coef} * {x}"
        else:
            expresion = f"{coef} * {x}^{grado} = {coef} * {potencia}"
        if len(expresion) > 30:
            expresion = expresion[:27] + "..."

        print(f"{paso:^6}{grado:^7}{expresion:<32}{termino:>12}{total:>12}")

    print("-" * ANCHO)
    print(f"  Resultado:  f({x}) = {total}")
    print(f"  Comprobacion: ({x} + 1)^{n} = {(x + 1) ** n}")
    print("=" * ANCHO)
    return total


def mostrar_resultado_python(n, x):
    """Genera y muestra polinomio + evaluación en Python."""
    coeficientes = generar_coeficientes(n)
    print("\nLenguaje: Python")
    print(f"Coeficientes (fila {n} de Pascal):")
    print("  " + "  ".join(str(c) for c in coeficientes))
    print(f"\n(x+1)^{n} =")
    imprimir_polinomio_ordenado(coeficientes)
    evaluar_paso_a_paso(coeficientes, x, etiqueta="Python")


def asegurar_ejecutable_c():
    """Si falta polinomio.exe, intenta compilarlo con gcc."""
    if os.path.isfile(EJECUTABLE_C):
        return True

    fuente = os.path.join(CARPETA, "polinomio.c")
    if not os.path.isfile(fuente):
        print("No se encontró polinomio.c")
        return False

    print("No está polinomio.exe. Compilando módulo C...")
    resultado = subprocess.run(
        ["gcc", "-Wall", "-Wextra", "-O2", "-o", EJECUTABLE_C, fuente],
        cwd=CARPETA,
        capture_output=True,
        text=True,
    )
    if resultado.returncode != 0:
        print("Error al compilar el módulo C:")
        print(resultado.stderr or resultado.stdout)
        print("Ejecute compilar.bat o instale gcc (MinGW).")
        return False

    print("Compilación exitosa.")
    return True


def ejecutar_modulo_c(argumentos):
    """Ejecuta polinomio.exe; None si no se pudo preparar."""
    if not asegurar_ejecutable_c():
        return None
    return subprocess.run(
        [EJECUTABLE_C, *argumentos],
        cwd=CARPETA,
        capture_output=True,
        text=True,
    )


def mostrar_resultado_c(n, x):
    """Delega el cálculo al módulo C y muestra su salida."""
    resultado = ejecutar_modulo_c(["calcular", str(n), str(x)])
    if resultado is None:
        return
    if resultado.returncode != 0:
        print("El módulo C terminó con error:")
        print(resultado.stderr or resultado.stdout)
        return
    print(resultado.stdout, end="")


def pedir_entero(mensaje, no_negativo=False):
    """Lee un entero de consola; None si es inválido."""
    try:
        valor = int(input(mensaje).strip())
    except ValueError:
        print("Debe ser un número entero.")
        return None
    if no_negativo and valor < 0:
        print("No puede ser negativo.")
        return None
    return valor


def modo_generar_polinomio():
    """Pide lenguaje, n y x; ejecuta Python o C y guarda resultado.txt."""
    print("\n--- Generar polinomio (x+1)^n ---")
    print("¿Con qué lenguaje desea calcular?")
    print("  1) Python")
    print("  2) C")
    eleccion = input("Opción: ").strip()

    n = pedir_entero("Ingrese n (entero no negativo): ", no_negativo=True)
    if n is None:
        return
    x = pedir_entero("Ingrese el valor de x para evaluar f(x): ")
    if x is None:
        return

    def _correr():
        print("--- Generar polinomio (x+1)^n ---")
        print(f"n = {n}")
        print(f"x = {x}")
        if eleccion == "1":
            mostrar_resultado_python(n, x)
        elif eleccion == "2":
            mostrar_resultado_c(n, x)
        else:
            print("Opción de lenguaje no válida.")

    if eleccion not in ("1", "2"):
        print("Opción de lenguaje no válida.")
        return

    con_resultado_en_archivo(_correr)


def _ejecutar_comparacion_tiempos():
    n = 100

    print()
    print("=" * ANCHO)
    print(f"  COMPARACION DE TIEMPOS  |  generar (x+1)^{n}")
    print("=" * ANCHO)
    print(f"  Metodo: promedio de {REPETICIONES_TIEMPO} generaciones (solo calculo)")
    print("  Nota: en C los coeficientes usan enteros grandes (n=100 no cabe en long).")

    print("\n[1/2] Generando con Python...")
    coeficientes = None
    inicio = time.perf_counter()
    for _ in range(REPETICIONES_TIEMPO):
        coeficientes = generar_coeficientes(n)
    tiempo_python = (time.perf_counter() - inicio) / REPETICIONES_TIEMPO
    print(f"      Listo. Coeficientes: {len(coeficientes)}")
    print(f"      Tiempo promedio Python: {tiempo_python:.6f} s")

    print("[2/2] Generando con C...")
    resultado = ejecutar_modulo_c(["tiempo"])
    if resultado is None:
        return
    if resultado.returncode != 0:
        print("Error al ejecutar el módulo C:")
        print(resultado.stderr or resultado.stdout)
        return

    tiempo_c = None
    for linea in resultado.stdout.splitlines():
        if linea.startswith("TIEMPO_C="):
            try:
                tiempo_c = float(linea.split("=", 1)[1])
            except ValueError:
                tiempo_c = None
            break
    if tiempo_c is None:
        print("No se pudo leer el tiempo del módulo C.")
        print(resultado.stdout)
        return

    print(f"      Listo. Tiempo promedio C: {tiempo_c:.6f} s")
    print()
    print("-" * ANCHO)
    print(f"{'Lenguaje':<12}{'Tiempo promedio (s)':>22}")
    print("-" * ANCHO)
    print(f"{'Python':<12}{tiempo_python:>22.6f}")
    print(f"{'C':<12}{tiempo_c:>22.6f}")
    print("-" * ANCHO)

    if tiempo_c > 0 and tiempo_python > 0:
        if tiempo_python >= tiempo_c:
            print(f"  Python tomo {tiempo_python / tiempo_c:.2f}x el tiempo de C")
        else:
            print(f"  C tomo {tiempo_c / tiempo_python:.2f}x el tiempo de Python")
        print("  (C puede ser mas lento aqui porque implementa aritmetica de")
        print("   enteros grandes a mano; Python ya trae enteros ilimitados.)")

    print()
    print(f"Polinomio (x+1)^{n} (version Python) =")
    imprimir_polinomio_ordenado(coeficientes)
    print("=" * ANCHO)


def comparar_tiempos_n100():
    """Compara tiempos y guarda resultado.txt."""
    con_resultado_en_archivo(_ejecutar_comparacion_tiempos)


def main():
    while True:
        print("\n===== EJERCICIO 2: Polinomio (x+1)^n =====")
        print("Programa único (Python + módulo C)")
        print("1) Generar polinomio y evaluar paso a paso")
        print("2) Comparar tiempos Python vs C (n=100)")
        print("0) Salir")

        opcion = input("Opción: ").strip()
        if opcion == "1":
            modo_generar_polinomio()
        elif opcion == "2":
            comparar_tiempos_n100()
        elif opcion == "0":
            print("Fin del programa.")
            break
        else:
            print("Opción no válida.")


if __name__ == "__main__":
    main()
