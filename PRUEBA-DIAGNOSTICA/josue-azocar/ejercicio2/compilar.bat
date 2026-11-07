@echo off
REM Compila el modulo C usado por polinomio.py
cd /d "%~dp0"
gcc -Wall -Wextra -O2 -o polinomio.exe polinomio.c
if %ERRORLEVEL% EQU 0 (
    echo Compilacion exitosa: polinomio.exe
    echo Ejecute el programa con: python polinomio.py
) else (
    echo Error al compilar. Verifique que gcc este en el PATH.
)
pause
