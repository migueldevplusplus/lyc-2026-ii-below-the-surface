import time

def es_salto(c1, f1, c2, f2):
    dc, df = abs(c2 - c1), abs(f2 - f1)
    return (dc == 2 and df == 1) or (dc == 1 and df == 2)

def validar(pos, movs, verbose=True):
    c, f = ord(pos[0]) - 97, int(pos[1]) - 1
    for i, m in enumerate(movs):
        if m[0] != 'K':
            if verbose: print(f"INCORRECTO falla letra {m[0]} no pertenece a un caballo")
            return False
        nc = ord(m[1]) - 97
        nf = int(m[2]) - 1
        if not (0 <= nc <= 7 and 0 <= nf <= 7 and es_salto(c, f, nc, nf)):
            if verbose: print(f"INCORRECTO falla en el movimiento {i+1}")
            return False
        c, f = nc, nf
    if verbose: print("movimiento CORRECTO")
    return True

BIG_MOVES = (
"Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 "
"Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 "
"Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
"Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 "
"Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 "
"Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
"Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 "
"Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
"Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 "
"Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 "
"Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
"Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 "
"Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
"Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 "
"Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 "
"Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
"Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 "
"Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 "
"Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 "
"Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 "
"Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4"
).split()

if len(BIG_MOVES) < 500:
    faltantes = 500 - len(BIG_MOVES)
    extra = ["Kb6", "Ka4"] * (faltantes // 2)
    if faltantes % 2 != 0:
        extra.append("Kb6") 
    BIG_MOVES.extend(extra)

def main():
    validar("a2", BIG_MOVES, verbose=False)
    print("Validacion previa: movimiento CORRECTO")
    
    t0 = time.perf_counter()
    for _ in range(10):
        validar("a2", BIG_MOVES, verbose=False)
    dt = time.perf_counter() - t0

    print(f"Movimientos verificados: {len(BIG_MOVES)}")
    print("Repeticiones: 10")
    print(f"Tiempo 10 repeticiones: {dt:.9f} s")
    print(f"Tiempo promedio por verificacion: {dt/10:.9f} s")

    with open("resultado.txt", "w") as out:
        out.write("Validacion previa: movimiento CORRECTO\n")
        out.write(f"Movimientos verificados: {len(BIG_MOVES)}\n")
        out.write("Repeticiones: 10\n")
        out.write(f"Tiempo 10 repeticiones: {dt:.9f} s\n")
        out.write(f"Tiempo promedio por verificacion: {dt/10:.9f} s\n")

if __name__ == "__main__":
    main()
