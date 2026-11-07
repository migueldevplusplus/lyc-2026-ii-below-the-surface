@echo off
REM Compila el Ejercicio 3 (Windows + gcc/MinGW)
cd /d "%~dp0"
gcc -Wall -Wextra -O2 -o reservadas.exe reservadas.c
if %ERRORLEVEL% EQU 0 (
    echo Compilacion exitosa: reservadas.exe
    echo Ejecute: reservadas.exe
) else (
    echo Error al compilar. Verifique que gcc este en el PATH.
)
pause
