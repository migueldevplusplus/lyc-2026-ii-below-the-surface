"""
Ejercicio 1 - Validación de movimientos del caballo (ajedrez).
Lenguaje: Python

Al finalizar cada ejecución útil, guarda la salida en resultado.txt
dentro de esta misma carpeta.
"""

import os
import sys
import time

# Desplazamientos válidos del caballo: (columna, fila)
SALTOS_CABALLO = (
    (-1, -2), (-1, 2), (1, -2), (1, 2),
    (-2, -1), (-2, 1), (2, -1), (2, 1),
)

COLUMNAS_VALIDAS = "abcdefgh"
FILAS_VALIDAS = "12345678"
ARCHIVO_500 = "movimientos_500.txt"
ARCHIVO_RESULTADO = "resultado.txt"
POSICION_INICIAL_500 = "a2"
REPETICIONES_MEDICION = 10
CARPETA = os.path.dirname(os.path.abspath(__file__))


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


def posicion_a_coordenadas(posicion):
    """Convierte notación de ajedrez (ej. 'a1') a coordenadas (columna, fila)."""
    columna = ord(posicion[0]) - ord("a")
    fila = int(posicion[1]) - 1
    return columna, fila


def es_posicion_valida(posicion):
    """Indica si la cadena es una casilla válida del tablero (ej. 'e4')."""
    return (
        len(posicion) == 2
        and posicion[0] in COLUMNAS_VALIDAS
        and posicion[1] in FILAS_VALIDAS
    )


def es_salto_de_caballo(origen, destino):
    """Indica si ir de origen a destino es un movimiento legal de caballo."""
    col_o, fila_o = posicion_a_coordenadas(origen)
    col_d, fila_d = posicion_a_coordenadas(destino)
    desplazamiento = (col_d - col_o, fila_d - fila_o)
    return desplazamiento in SALTOS_CABALLO


def parsear_movimiento(texto):
    """
    Separa un token tipo 'Kc2' en (pieza, casilla).
    Devuelve (None, mensaje) si el formato es inválido.
    """
    texto = texto.strip()
    if len(texto) != 3:
        return None, f"formato inválido '{texto}' (se espera letra + columna + fila)"

    pieza = texto[0]
    casilla = texto[1:]
    return (pieza, casilla), None


def verificar_movimientos(posicion_inicial, movimientos, n=None):
    """
    Recorre la lista de movimientos desde la posición inicial.
    Se detiene al primer error.

    Si se indica n, la cadena debe tener exactamente n movimientos
    (tal como pide el enunciado: Ak, n y C).

    Retorna (es_correcto, mensaje).
    """
    if not es_posicion_valida(posicion_inicial):
        return False, f"posición inicial inválida: {posicion_inicial}"

    if n is not None:
        if n < 0:
            return False, "INCORRECTO: n no puede ser negativo."
        if len(movimientos) != n:
            return (
                False,
                f"INCORRECTO: n={n} pero la cadena tiene {len(movimientos)} "
                f"movimiento(s). Deben coincidir exactamente.",
            )

    posicion_actual = posicion_inicial.lower()

    for indice, token in enumerate(movimientos, start=1):
        parseado, error_formato = parsear_movimiento(token)
        if error_formato:
            return False, f"INCORRECTO falla en el movimiento {indice}: {error_formato}"

        pieza, casilla = parseado
        casilla = casilla.lower()

        if pieza != "K":
            return (
                False,
                f"INCORRECTO falla letra {pieza} no pertenece a un caballo.",
            )

        if not es_posicion_valida(casilla):
            return (
                False,
                f"INCORRECTO falla en el movimiento {indice}: casilla inválida {casilla}",
            )

        if not es_salto_de_caballo(posicion_actual, casilla):
            return False, f"INCORRECTO falla en el movimiento {indice}."

        posicion_actual = casilla

    return True, "movimiento CORRECTO."


def cargar_movimientos_500():
    """Lee los 500 movimientos del archivo adjunto a este ejercicio."""
    ruta = os.path.join(CARPETA, ARCHIVO_500)
    with open(ruta, "r", encoding="utf-8") as archivo:
        contenido = archivo.read()
    return contenido.split()


def _ejecutar_medicion_500():
    movimientos = cargar_movimientos_500()
    print(f"\nMovimientos cargados: {len(movimientos)}")
    print(f"Posición inicial: {POSICION_INICIAL_500}")
    print(f"Repeticiones: {REPETICIONES_MEDICION}\n")

    correcto, mensaje = verificar_movimientos(POSICION_INICIAL_500, movimientos)
    print(f"Resultado de la secuencia: {mensaje}")

    if not correcto:
        print("No se mide el tiempo porque la secuencia contiene errores.")
        return

    inicio = time.perf_counter()
    for _ in range(REPETICIONES_MEDICION):
        verificar_movimientos(POSICION_INICIAL_500, movimientos)
    fin = time.perf_counter()

    total = fin - inicio
    print(f"Tiempo total ({REPETICIONES_MEDICION} verificaciones): {total:.6f} segundos")
    print(f"Tiempo promedio por verificación: {total / REPETICIONES_MEDICION:.6f} segundos")


def medir_tiempo_500():
    """Verifica 10 veces los 500 movimientos, muestra y guarda resultado.txt."""
    con_resultado_en_archivo(_ejecutar_medicion_500)


def modo_interactivo():
    """Pide posición inicial, n y la cadena; guarda el resultado en resultado.txt."""
    print("\n--- Validación interactiva ---")
    posicion = input("Posición inicial del caballo (ej. a1): ").strip().lower()

    try:
        n = int(input("Cantidad de movimientos (n): ").strip())
    except ValueError:
        print("n debe ser un número entero.")
        return

    if n < 0:
        print("n no puede ser negativo.")
        return

    cadena = input(
        "Cadena de movimientos separados por espacio (ej. Kc2 Kd4 Kb3 Kc5): "
    ).strip()

    movimientos = cadena.split()

    def _correr():
        print("--- Validación interactiva ---")
        print(f"Posición inicial (Ak): {posicion}")
        print(f"n: {n}")
        print(f"Cadena C: {cadena}")
        _, mensaje = verificar_movimientos(posicion, movimientos, n=n)
        print(mensaje)

    con_resultado_en_archivo(_correr)


def main():
    while True:
        print("\n===== EJERCICIO 1: Movimientos del caballo =====")
        print("1) Validar movimientos (consola interactiva)")
        print("2) Medir tiempo de los 500 movimientos (10 veces)")
        print("0) Salir")

        opcion = input("Opción: ").strip()

        if opcion == "1":
            modo_interactivo()
        elif opcion == "2":
            medir_tiempo_500()
        elif opcion == "0":
            print("Fin del programa.")
            break
        else:
            print("Opción no válida.")


if __name__ == "__main__":
    main()
