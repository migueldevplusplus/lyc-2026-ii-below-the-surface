#!/bin/bash
echo "=== INICIANDO INSTALACIÓN DE DEPENDENCIAS ==="

# 1. Verificar e instalar GCC (C)
if command -v gcc &>/dev/null; then
    echo "[OK] GCC ya está instalado."
else
    echo "[!] GCC no encontrado. Instalando..."
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        sudo apt-get update && sudo apt-get install -y build-essential
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        xcode-select --install
    fi
fi

# 2. Verificar e instalar Python 3
if command -v python3 &>/dev/null; then
    echo "[OK] Python 3 ya está instalado."
else
    echo "[!] Python 3 no encontrado. Instalando..."
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        sudo apt-get install -y python3
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        brew install python3
    fi
fi

# 3. Compilar los archivos en C
echo "=== COMPILANDO PROGRAMAS EN C ==="
gcc -O2 -o ejercicio1/caballo ejercicio1/caballo.c
gcc -O2 -o ejercicio2/pascal ejercicio2/pascal.c
gcc -O2 -o ejercicio3/traductor ejercicio3/traductor.c

echo "=== INSTALACIÓN Y COMPILACIÓN COMPLETADAS ==="