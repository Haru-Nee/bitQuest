@echo off
echo Compilando BitQuest...

echo [1/2] Ensamblando rutinas.asm...
nasm -f win64 rutinas.asm -o rutinas.obj
if errorlevel 1 (
    echo ERROR: Fallo al ensamblar rutinas.asm
    pause
    exit /b 1
)

echo [2/2] Compilando y enlazando...
gcc main.c juego.c mapas.c rutinas.obj -o BitQuest.exe
if errorlevel 1 (
    echo ERROR: Fallo al compilar
    pause
    exit /b 1
)

echo.
echo Compilacion exitosa!
echo Ejecutando BitQuest...
echo.
BitQuest.exe