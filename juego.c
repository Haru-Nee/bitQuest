#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "juego.h"
#include "mapas.h"

/*Imprimir ventana 20*20, pov del jugador*/
void imprimir_ventana(const GameState *gs){

    int cam_row, cam_col;
    int i, j;
    char celda;

    /*Esquina superior izquierda de la camara*/
    cam_row = gs->jugador.fila - (VIEW_ROWS/2);
    cam_col = gs->jugador.col - (VIEW_COLS/2);

    if(cam_row<0) cam_row=0;
    if(cam_col<0) cam_col=0;
    if(cam_row > MAP_ROWS - VIEW_ROWS) cam_row = MAP_ROWS - VIEW_ROWS;
    if(cam_col > MAP_COLS - VIEW_COLS) cam_col = MAP_COLS - VIEW_COLS;

    /*Imprimir las celdas visibles*/
    for(i=0; i <VIEW_ROWS; i++){
        for(j=0; j<VIEW_COLS; j++){
            celda = gs->mapa.celdas[(cam_row+i)*MAP_COLS+(cam_col+j)];
            
            /*dibujar al jugador en su posicion real*/
            if ((cam_row + i) == gs->jugador.fila &&
                (cam_col + j) == gs->jugador.col) {
                printf("%sP%s", COLOR_JUGADOR, COLOR_RESET);
            } else {
                switch (celda) {
                    case '#': printf("%s#%s", COLOR_PARED,   COLOR_RESET); break;
                    case '.': printf("%s.%s", COLOR_LIBRE,   COLOR_RESET); break;
                    case 'M': printf("%sM%s", COLOR_MONEDA,  COLOR_RESET); break;
                    case 'K': printf("%sK%s", COLOR_LLAVE,   COLOR_RESET); break;
                    case 'D': printf("%sD%s", COLOR_PUERTA,  COLOR_RESET); break;
                    case 'E': printf("%sE%s", COLOR_SALIDA,  COLOR_RESET); break;
                    default:  printf("%c", celda);                          break;
                }
            }
        }
        printf("\n");
    }
}

/*Mostrar HUD*/
void mostrar_hud(const GameState *gs) {

    printf("================================\n");
    printf("  Nivel : %d\n", gs->nivel);
    printf("  Llave : %s\n", gs->tiene_llave ? "Si" : "No");
    printf("  Pasos : %d\n", gs->pasos);
    printf("  Monedas: %d / %d\n", gs->monedas_recogidas, gs->monedas_totales);
    printf("================================\n");
}

/*Resumen al terminar un nivel*/
void mostrar_resumen_nivel(const GameState *gs) {

    printf("\n================================\n");
    printf("  Nivel %d completado!\n", gs->nivel);
    printf("  Monedas recolectadas: %d / %d\n",
            gs->monedas_recogidas, gs->monedas_totales);
    printf("  Pasos realizados    : %d\n", gs->pasos);
    printf("================================\n\n");

    printf("Presiona cualquier tecla para continuar...\n");
    getchar();
}

/* Resumen final al terminar los 3 niveles*/
void mostrar_resumen_final(int mon_total, int mon_posibles,int pasos_total, int niveles, int puntaje) {

    printf("\n================================\n");
    printf("  Juego completado!\n");
    printf("  Monedas totales    : %d / %d\n", mon_total, mon_posibles);
    printf("  Pasos totales      : %d\n", pasos_total);
    printf("  Niveles completados: %d\n", niveles);
    printf("  Puntaje final      : %d\n", puntaje);
    printf("================================\n\n");
}

/*Procesar tecla*/
int procesar_tecla(int tecla, GameState *gs){
    int nueva_fila=gs->jugador.fila;
    int nueva_col=gs->jugador.col;
    char celda;
    int total=MAP_ROWS*MAP_COLS;

    /*Calcular posicion segun la tecla*/
    switch (tecla) {
        case 'w': case 'W': nueva_fila--; break;
        case 's': case 'S': nueva_fila++; break;
        case 'a': case 'A': nueva_col--;  break;
        case 'd': case 'D': nueva_col++;  break;
        case 'q': case 'Q': return -1;   /* salir */
        default: return 0;
    }

    /* NASM: validar que no sea pared */
    if (!validar_movimiento(gs->mapa.celdas, MAP_COLS, nueva_fila, nueva_col)) {
        return 0;  /* movimiento bloqueado */
    }

    /* Obtener celda destino */
    celda = gs->mapa.celdas[nueva_fila * MAP_COLS + nueva_col];

    /* Verificar puerta sin llave */
    if (celda == SYM_DOOR && !gs->tiene_llave) {
        printf("  Necesitas la llave!\n");
        return 0;
    }

    /* NASM: detectar moneda */
    if (detectar_objeto(gs->mapa.celdas, MAP_COLS, nueva_fila, nueva_col, SYM_COIN)) {
        gs->monedas_recogidas++;
        gs->mapa.celdas[nueva_fila * MAP_COLS + nueva_col] = SYM_FREE;
    }

    /* NASM: detectar llave */
    if (detectar_objeto(gs->mapa.celdas, MAP_COLS, nueva_fila, nueva_col, SYM_KEY)) {
        gs->tiene_llave = 1;
        gs->mapa.celdas[nueva_fila * MAP_COLS + nueva_col] = SYM_FREE;
        printf("  Llave recogida!\n");
    }

    /* NASM: detectar salida */
    if (detectar_objeto(gs->mapa.celdas, MAP_COLS, nueva_fila, nueva_col, SYM_EXIT)) {
        gs->jugador.fila = nueva_fila;
        gs->jugador.col  = nueva_col;
        gs->pasos++;
        return 2;  /* nivel completado */
    }

     /* Mover jugador */
    gs->jugador.fila = nueva_fila;
    gs->jugador.col  = nueva_col;
    gs->pasos++;

    return 1;  /* movimiento exitoso */
}

/* ─── Limpiar pantalla ─── */
void limpiar_pantalla(void) {
    system("cls");
}

/* Habilitar colores ANSI en Windows */
void habilitar_colores(void) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

/* Retorna el color del nivel actual */
const char* color_nivel(int nivel) {
    switch (nivel) {
        case 1:  return COLOR_NIV1;
        case 2:  return COLOR_NIV2;
        case 3:  return COLOR_NIV3;
        default: return COLOR_RESET;
    }
}