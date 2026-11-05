#!/usr/bin/env bash
# Instala los componentes necesarios: compilador de C (gcc) e interprete de Python 3.
set -e
if command -v dnf >/dev/null 2>&1; then
    sudo dnf install -y gcc python3          # Fedora / RHEL
elif command -v apt-get >/dev/null 2>&1; then
    sudo apt-get update && sudo apt-get install -y gcc python3   # Debian / Ubuntu
else
    echo "Instale gcc y python3 con el gestor de paquetes de su sistema."
    exit 1
fi
gcc --version | head -1
python3 --version
