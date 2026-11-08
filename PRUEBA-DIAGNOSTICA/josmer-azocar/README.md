# Prueba Diagnostica - Josmer Azocar

**Video de la defensa:**

## Instalacion

```bash
./instalar.sh
```

Instala Python 3 y GCC, y compila los programas en C.

## Ejercicios

### Ejercicio 1 - Movimientos del caballo (Python)

```bash
cd ejercicio1
python3 caballo.py
```

- Opcion 1: verifica los movimientos ingresados por el usuario.
- Opcion 2: mide el tiempo de verificar 10 veces los 500 movimientos de `movimientos.txt` y lo guarda en `resultados.txt`.

### Ejercicio 2 - Triangulo de Pascal (Python y C)

```bash
cd ejercicio2
python3 pascal.py
./pascal
```

- Opcion 1: genera el polinomio (x+1)^n y calcula f(x) por pasos.
- Opcion 2: mide el tiempo de generar el polinomio para n=100 y lo guarda en `resultados_python.txt` o `resultados_c.txt`.

### Ejercicio 3 - Traductor de palabras reservadas (C)

```bash
cd ejercicio3
./traductor
```

Carga un archivo C en memoria dinamica, traduce sus palabras reservadas al espanol y guarda el resultado en `traducido.txt`.
