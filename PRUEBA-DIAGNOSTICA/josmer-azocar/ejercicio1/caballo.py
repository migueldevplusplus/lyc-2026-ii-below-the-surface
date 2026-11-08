import time

COLUMNAS = "abcdefgh"
FILAS = "12345678"


def es_casilla_valida(casilla):
    return len(casilla) == 2 and casilla[0] in COLUMNAS and casilla[1] in FILAS


def es_salto_de_caballo(origen, destino):
    # El caballo se mueve en "L": 2 casillas en un sentido y 1 en el otro
    dif_col = abs(COLUMNAS.index(origen[0]) - COLUMNAS.index(destino[0]))
    dif_fila = abs(FILAS.index(origen[1]) - FILAS.index(destino[1]))
    return (dif_col == 1 and dif_fila == 2) or (dif_col == 2 and dif_fila == 1)


def verificar_movs(pos_inicial, cadena):
    movs = cadena.split()
    pos_actual = pos_inicial

    for num, mov in enumerate(movs, start=1):
        if mov[0] != "K":
            return "INCORRECTO falla letra " + mov[0] + " no pertenece a un caballo"

        casilla = mov[1:]
        if not es_casilla_valida(casilla):
            return "INCORRECTO falla en el movimiento " + str(num)

        if not es_salto_de_caballo(pos_actual, casilla):
            return "INCORRECTO falla en el movimiento " + str(num)

        pos_actual = casilla

    return "movimiento CORRECTO"


def verificar_entrada_usuario():
    pos = input("Posicion inicial (ej. a1): ").strip()
    if not es_casilla_valida(pos):
        print("  Posicion inicial invalida, debe ser columna a-h y fila 1-8")
        return
    cadena = input("Movimientos (ej. Kc2 Kd4 Kb3): ").strip()
    print("  n =", len(cadena.split()), " salida:", verificar_movs(pos, cadena))


def prueba_de_tiempo():
    with open("movimientos.txt") as archivo:
        cadena_500 = archivo.read()

    texto = "Posicion inicial: a2\n"
    texto += "Cantidad de movimientos: " + str(len(cadena_500.split())) + "\n\n"

    tiempo_total = 0
    for i in range(1, 11):
        inicio = time.perf_counter()
        resultado = verificar_movs("a2", cadena_500)
        fin = time.perf_counter()

        tiempo_ms = (fin - inicio) * 1000
        tiempo_total += tiempo_ms
        texto += "Iteracion " + str(i) + ": " + resultado + " - " + str(round(tiempo_ms, 4)) + " ms\n"

    texto += "\nTiempo total de las 10 verificaciones: " + str(round(tiempo_total, 4)) + " ms\n"

    print(texto)
    with open("resultados.txt", "w") as archivo:
        archivo.write(texto)
    print("  Resultados guardados en resultados.txt")


while True:
    print()
    print("1. Verificar movimientos del caballo")
    print("2. Medir tiempo de verificar 10 veces los 500 movimientos")
    print("0. Salir")
    opcion = input("Opcion: ").strip()

    if opcion == "1":
        verificar_entrada_usuario()
    elif opcion == "2":
        prueba_de_tiempo()
    elif opcion == "0":
        break
    else:
        print("  Opcion invalida")
