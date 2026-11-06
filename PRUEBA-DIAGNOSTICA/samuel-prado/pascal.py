import sys, time

def generar(n):
    c = [0] * (n + 1)
    c[0] = 1
    for i in range(1, n + 1):
        for j in range(i, 0, -1):
            c[j] += c[j - 1]
    return c

def mostrar_polinomio(c, n):
    print(f"(x+1)^{n} = ", end="")
    primero = True
    for k in range(n + 1):
        pot = n - k
        if c[k] == 0:
            continue
        if not primero:
            print(" + ", end="")
        primero = False
        print(c[k], end="")
        if pot >= 1:
            print("*x", end="")
            if pot > 1:
                print(f"^{pot}", end="")
    print()
    print("Coeficientes: " + " ".join(str(v) for v in c))

def paso_a_paso(c, n, x):
    print(f"\nPaso a paso f({x}) = ({x} + 1)^{n}:")
    suma = 0
    for k in range(n + 1):
        pot = n - k
        term = c[k] * (x ** pot)
        print(f"  term[{k}] = coef[{k}] * {x}^{pot} = {term}")
        suma += term
    print(f"  f({x}) = {suma}")

def medir(n, reps):
    t0 = time.perf_counter()
    for _ in range(reps):
        generar(n)
    return time.perf_counter() - t0

if __name__ == "__main__":
    n = int(sys.argv[1]) if len(sys.argv) >= 2 else 5
    x = int(sys.argv[2]) if len(sys.argv) >= 3 else 2

    c = generar(n)
    mostrar_polinomio(c, n)
    paso_a_paso(c, n, x)

    dt = medir(100, 10)
    print(f"\nTiempo generacion n=100 (10 repeticiones): {dt:.9f} s")
    print(f"Tiempo promedio por generacion: {dt/10:.9f} s")

    with open("resultado.txt", "w") as f:
        f.write("Lenguaje: Python\n")
        f.write(f"Tiempo n=100 (10 repeticiones): {dt:.9f} s\n")
        f.write(f"Tiempo promedio: {dt/10:.9f} s\n")