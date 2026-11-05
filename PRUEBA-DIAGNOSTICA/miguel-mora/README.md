# Prueba Diagnóstica - Miguel Mora

**Grupo:** Below the Surface. *Más allá de la interfaz.* · **Sección:** 01

**Video de la defensa:** https://drive.google.com/file/d/1SxNjcrbmn3FBLcC9pp3Y8ZBplXif1yqH/view?usp=sharing

## Instalación

```bash
./instalar.sh        # instala gcc y python3
```

## Ejecución

**Ejercicio 1 — Validador de caballo**
```bash
cd ejercicio1
python3 caballo.py
gcc -O2 caballo.c -o caballo && ./caballo
```

**Ejercicio 2 — Triángulo de Pascal**
```bash
cd ejercicio2
python3 pascal.py
gcc -O2 pascal.c -o pascal && ./pascal
```

**Ejercicio 3 — Traductor de palabras reservadas**
```bash
cd ejercicio3
python3 traductor.py
gcc -O2 traductor.c -o traductor && ./traductor
```
