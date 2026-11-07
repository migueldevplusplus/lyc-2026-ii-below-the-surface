@echo off
REM ============================================================
REM Script de instalacion / preparacion - Prueba Diagnostica
REM Lenguajes y Compiladores - Josue Azocar
REM
REM Verifica Python y gcc, y compila los ejercicios en C.
REM Uso: doble clic, o desde consola: instalar.bat
REM ============================================================

setlocal EnableExtensions
cd /d "%~dp0"

set "ERRORES=0"

echo.
echo ============================================================
echo   INSTALACION / PREPARACION DE LA PRUEBA DIAGNOSTICA
echo ============================================================
echo   Carpeta: %CD%
echo.

REM ---------- 1) Python (Ejercicio 1 y 2) ----------
echo [1/3] Verificando Python...
where python >nul 2>&1
if errorlevel 1 (
    echo   ERROR: no se encontro "python" en el PATH.
    echo   Instale Python 3 y marque la opcion "Add to PATH".
    set "ERRORES=1"
) else (
    for /f "delims=" %%V in ('python --version 2^>^&1') do echo   OK: %%V
)

echo.

REM ---------- 2) gcc / MinGW (Ejercicio 2 y 3) ----------
echo [2/3] Verificando compilador gcc...
where gcc >nul 2>&1
if errorlevel 1 (
    echo   ERROR: no se encontro "gcc" en el PATH.
    echo   Instale MinGW o MSYS2 y agregue gcc al PATH.
    set "ERRORES=1"
) else (
    for /f "delims=" %%V in ('gcc --version 2^>^&1') do (
        echo   OK: %%V
        goto :gcc_ok
    )
)
:gcc_ok

echo.

REM ---------- 3) Compilar modulos en C ----------
echo [3/3] Compilando ejercicios en C...

if "%ERRORES%"=="1" (
    echo   Se omiten compilaciones porque faltan herramientas.
    goto :resumen
)

echo   - Ejercicio 2: polinomio.c
gcc -Wall -Wextra -O2 -o "ejercicio2\polinomio.exe" "ejercicio2\polinomio.c"
if errorlevel 1 (
    echo     ERROR al compilar ejercicio2\polinomio.c
    set "ERRORES=1"
) else (
    echo     OK: ejercicio2\polinomio.exe
)

echo   - Ejercicio 3: reservadas.c
gcc -Wall -Wextra -O2 -o "ejercicio3\reservadas.exe" "ejercicio3\reservadas.c"
if errorlevel 1 (
    echo     ERROR al compilar ejercicio3\reservadas.c
    set "ERRORES=1"
) else (
    echo     OK: ejercicio3\reservadas.exe
)

:resumen
echo.
echo ============================================================
if "%ERRORES%"=="0" (
    echo   INSTALACION COMPLETADA CON EXITO
    echo.
    echo   Como ejecutar:
    echo     Ejercicio 1:  python ejercicio1\caballo.py
    echo     Ejercicio 2:  python ejercicio2\polinomio.py
    echo     Ejercicio 3:  ejercicio3\reservadas.exe
) else (
    echo   INSTALACION CON ERRORES
    echo   Revise los mensajes anteriores e intente de nuevo.
)
echo ============================================================
echo.

REM Si se llama: instalar.bat nopause  -> no espera tecla (util para pruebas)
if /i not "%~1"=="nopause" pause

if "%ERRORES%"=="1" (
    endlocal
    exit /b 1
)
endlocal
exit /b 0
