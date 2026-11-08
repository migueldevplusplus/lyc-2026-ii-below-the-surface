#!/bin/bash
# Instala Python 3 y GCC (Debian/Ubuntu) y compila los programas en C

sudo apt-get update
sudo apt-get install -y python3 gcc

gcc -o ejercicio2/pascal ejercicio2/pascal.c -lm
gcc -o ejercicio3/traductor ejercicio3/traductor.c

echo "Instalacion completa"
