# bitQuest

Este es un juego de exploración de mazmorras en 2D que corre directamente en la consola de Windows. El reto principal de este proyecto fue armarlo de forma híbrida: toda la lógica principal, el HUD, la carga de mapas y el renderizado los trabajamos en **C**, pero para las funciones de bajo nivel (procesamiento de matrices, validación de colisiones y el cálculo del score) programamos rutinas optimizadas en **Ensamblador (NASM x86-64)**.

---

## 🛠️ Organización del Código

Para no volvernos locos con el código, decidimos modularizar el proyecto de la siguiente manera:

*   **`main.c`**: Contiene el bucle principal del juego y controla la transición entre los 3 niveles.
*   **`juego.c` / `juego.h`**: Aquí controlamos lo que ve el usuario. Como los mapas completos miden 60x60, implementamos una cámara de 20x20 que sigue la posición del jugador para que sea cómodo de jugar. También habilitamos los colores ANSI para que la terminal resalte las paredes, monedas y llaves.
*   **`mapas.c` / `mapas.h`**: Aquí nos encargamos de la lectura de los archivos `.txt` de los niveles, la asignación de memoria dinámica y la liberación de recursos al cerrar el juego.
*   **`rutinas.asm`**: El archivo de Ensamblador. Programamos estas funciones en limpio usando la convención de llamadas de Windows x64:
    *   `contar_caracteres` y `contar_celdas_libres`: Recorren la matriz del mapa para registrar el total de monedas y espacios transitables.
    *   `validar_movimiento` y `detectar_objeto`: Verifican colisiones con paredes (`#`) o si el jugador pisa algún ítem.
    *   `calcular_puntaje`: Saca el puntaje final basado en el rendimiento.

---

## 🎮 Mecánicas del Juego

*   El jugador se mueve usando las teclas **`W`, `A`, `S`, `D`**.
*   Se puede salir de la partida en cualquier momento presionando **`Q`**.
*   El objetivo es recolectar las monedas (**`M`**) y llegar a la salida (**`E`**). Si el camino está bloqueado por una puerta (**`D`**), primero hay que encontrar la llave (**`K`**) por el mapa.

---

## 💻 Compilación y Ejecución

El proyecto está diseñado para compilarse en Windows usando **NASM** y **GCC (MinGW)**. 

Los comandos que usamos en la terminal para armar el ejecutable son:

1. Compilar el archivo de ensamblador a objeto: usamos el archivo build.bat
2. Ejecutar el .exe
3. Listo para jugar
