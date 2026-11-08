import time


def generar_coefs(n):
    # Cada fila nueva se crea a partir de la anterior
    fila = [1]
    for i in range(1, n + 1):
        nueva = [1]
        for j in range(1, i):
            nueva.append(fila[j - 1] + fila[j])
        nueva.append(1)
        fila = nueva
    return fila


def polinomio_a_texto(coefs):
    n = len(coefs) - 1
    texto = ""
    for k in range(n + 1):
        exp = n - k
        if k > 0:
            texto += "+"
        if coefs[k] != 1 or exp == 0:
            texto += str(coefs[k])
        if exp >= 1:
            texto += "x"
        if exp >= 2:
            texto += "^" + str(exp)
    return texto


def evaluar_por_pasos(coefs, x):
    n = len(coefs) - 1
    acumulado = 0
    for k in range(n + 1):
        exp = n - k
        termino = coefs[k] * x ** exp
        acumulado += termino
        print("  Paso", k + 1, ":", coefs[k], "*", str(x) + "^" + str(exp), "=", termino,
              "  acumulado =", acumulado)
    print("  f(" + str(x) + ") =", acumulado)
    print("  Comprobacion: (" + str(x) + "+1)^" + str(n), "=", (x + 1) ** n)


def generar_y_evaluar():
    texto_n = input("n (entero no negativo): ").strip()
    if not texto_n.isdigit():
        print("  n invalido")
        return
    n = int(texto_n)

    coefs = generar_coefs(n)
    print("  Coeficientes:", coefs)
    print("  (x+1)^" + str(n), "=", polinomio_a_texto(coefs))

    try:
        x = int(input("x (entero): ").strip())
    except ValueError:
        print("  x invalido")
        return
    evaluar_por_pasos(coefs, x)


def prueba_de_tiempo():
    n = 100
    inicio = time.perf_counter()
    coefs = generar_coefs(n)
    fin = time.perf_counter()
    tiempo_ms = (fin - inicio) * 1000

    texto = "Lenguaje: Python\n"
    texto += "n = " + str(n) + "\n"
    texto += "Tiempo de generacion: " + str(round(tiempo_ms, 4)) + " ms\n"
    texto += "\n(x+1)^" + str(n) + " = " + polinomio_a_texto(coefs) + "\n"

    print(texto)
    with open("resultados_python.txt", "w") as archivo:
        archivo.write(texto)
    print("  Resultados guardados en resultados_python.txt")


while True:
    print()
    print("1. Generar polinomio (x+1)^n y calcular f(x)")
    print("2. Medir tiempo de generar el polinomio para n=100")
    print("0. Salir")
    opcion = input("Opcion: ").strip()

    if opcion == "1":
        generar_y_evaluar()
    elif opcion == "2":
        prueba_de_tiempo()
    elif opcion == "0":
        break
    else:
        print("  Opcion invalida")
