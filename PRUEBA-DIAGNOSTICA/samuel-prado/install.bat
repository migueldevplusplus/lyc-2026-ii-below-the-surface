@echo off
echo === INICIANDO VERIFICACION DE DEPENDENCIAS ===

:: 1. Verificar GCC
where gcc >nul 2>nul
if %errorlevel% neq 0 (
    echo [!] GCC no encontrado.
    echo Por favor, instala MinGW-w64 o Dev-C++ y agrega gcc al PATH de Windows.
    echo Descarga: https://www.mingw-w64.org/
    pause
    exit /b
) else (
    echo [OK] GCC encontrado.
)

:: 2. Verificar Python
where python >nul 2>nul
if %errorlevel% neq 0 (
    echo [!] Python no encontrado.
    echo Por favor, instala Python 3 desde https://www.python.org/downloads/
    echo Asegurate de marcar la casilla "Add Python to PATH".
    pause
    exit /b
) else (
    echo [OK] Python encontrado.
)

:: 3. Compilar los archivos en C
echo === COMPILANDO PROGRAMAS EN C ===
gcc -O2 -o ejercicio1\caballo.exe ejercicio1\caballo.c
gcc -O2 -o ejercicio2\pascal.exe ejercicio2\pascal.c
gcc -O2 -o ejercicio3\traductor.exe ejercicio3\traductor.c

echo === COMPILACION COMPLETADA CON EXITO ===
pause