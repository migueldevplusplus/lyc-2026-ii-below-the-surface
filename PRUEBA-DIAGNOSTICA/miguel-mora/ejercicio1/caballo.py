"""
Ejercicio 1 - Validador de movimientos de caballo (Python)

Un salto de caballo es valido si, entre la casilla de origen y la de destino,
una coordenada cambia en 1 y la otra en 2 (forma de "L").
El programa se detiene en el primer movimiento incorrecto.

Uso:
    python3 caballo.py                      -> corre los ejemplos y las mediciones
    python3 caballo.py a1 "Kc2 Kd4 Kb3"     -> valida una entrada propia
"""
import os
import sys
import time

COLUMNAS = "abcdefgh"


def casilla_a_coordenadas(casilla):
    """Convierte 'c2' en (columna, fila) = (2, 2). Devuelve None si no existe."""
    if len(casilla) != 2:
        return None
    letra_columna, digito_fila = casilla[0], casilla[1]
    if letra_columna not in COLUMNAS or digito_fila not in "12345678":
        return None
    return COLUMNAS.index(letra_columna) + 1, int(digito_fila)


def es_salto_de_caballo(origen, destino):
    diferencia_columnas = abs(origen[0] - destino[0])
    diferencia_filas = abs(origen[1] - destino[1])
    return (diferencia_columnas == 1 and diferencia_filas == 2) or \
        (diferencia_columnas == 2 and diferencia_filas == 1)


def validar_movimientos(posicion_inicial, cadena_movimientos):
    """Recorre los movimientos uno a uno y se detiene en el primer error."""
    posicion_actual = casilla_a_coordenadas(posicion_inicial)
    if posicion_actual is None:
        return f"INCORRECTO posicion inicial {posicion_inicial} no existe"

    movimientos = cadena_movimientos.split()
    for numero, movimiento in enumerate(movimientos, start=1):
        letra_pieza = movimiento[0]
        if letra_pieza != "K":
            return (f"movimiento INCORRECTO falla letra {letra_pieza} "
                    f"no pertenece a un caballo (movimiento {numero})")

        destino = casilla_a_coordenadas(movimiento[1:])
        if destino is None:
            return f"INCORRECTO falla en el movimiento {numero} (casilla fuera del tablero)"

        if not es_salto_de_caballo(posicion_actual, destino):
            return f"INCORRECTO falla en el movimiento {numero}"

        posicion_actual = destino

    return "movimiento CORRECTO"


EJEMPLOS = [
    ("a1", "Kc2 Kd4 Kb3 Kc5"),
    ("a1", "Kc2 Kd4 Kb4 Kc5"),
    ("a2", "Kc3 Ke4 Kf6 Ke8 Kg7"),
    ("a2", "Kc8 Ke4 Kf6 Ke8 Kg7"),
    ("a8", "Kb6 Kd7"),
    ("a8", "Kb6 Kb7"),
    ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4"),
    ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4"),
    ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4"),
]

# El enunciado no da la casilla inicial de la secuencia larga.
# Kb4 es alcanzable desde a2, a6, c2, c6, d3 o d5: se asume a2.
POSICION_INICIAL_SECUENCIA = "a2"
REPETICIONES = 10


def main():
    # Trabaja en la carpeta del script, para encontrar sus archivos desde donde sea
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    if len(sys.argv) == 3:
        print(validar_movimientos(sys.argv[1], sys.argv[2]))
        return

    lineas_salida = ["=== Ejercicio 1 - Validador de caballo (Python) ===", ""]

    for posicion_inicial, cadena in EJEMPLOS:
        cantidad = len(cadena.split())
        resultado = validar_movimientos(posicion_inicial, cadena)
        lineas_salida.append(f"Ak={posicion_inicial} n={cantidad} C={cadena}")
        lineas_salida.append(f"  salida: {resultado}")

    with open("secuencia.txt", encoding="utf-8") as archivo:
        secuencia_completa = " ".join(archivo.read().split())
    movimientos_secuencia = secuencia_completa.split()
    primeros_100 = " ".join(movimientos_secuencia[:100])

    # n = 100 movimientos
    inicio = time.perf_counter()
    resultado_100 = validar_movimientos(POSICION_INICIAL_SECUENCIA, primeros_100)
    tiempo_100 = time.perf_counter() - inicio

    # Secuencia completa verificada 10 veces
    inicio = time.perf_counter()
    for _ in range(REPETICIONES):
        resultado_completo = validar_movimientos(POSICION_INICIAL_SECUENCIA, secuencia_completa)
    tiempo_completo = time.perf_counter() - inicio

    lineas_salida += [
        "",
        f"Posicion inicial asumida para la secuencia: {POSICION_INICIAL_SECUENCIA}",
        f"n = 100 movimientos: {resultado_100}",
        f"  tiempo: {tiempo_100 * 1e6:.2f} microsegundos",
        f"Secuencia del enunciado ({len(movimientos_secuencia)} movimientos) x {REPETICIONES}: {resultado_completo}",
        f"  tiempo total: {tiempo_completo * 1e6:.2f} microsegundos",
    ]

    texto = "\n".join(lineas_salida)
    print(texto)
    with open("resultados_python.txt", "w", encoding="utf-8") as archivo:
        archivo.write(texto + "\n")


if __name__ == "__main__":
    main()
