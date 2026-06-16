#include <stdio.h>
#include <stdlib.h>
#include <conio.h>    /* _getch() para Windows */
#include "juego.h"
#include "mapas.h"

int main(void) {

    GameState gs;
    int resultado;
    int tecla;
    int mon_posibles_total = 0;
    int mon_recogidas_total = 0;
    int pasos_total = 0;
    int puntaje;

    const char *archivos[] = { "nivel1.txt", "nivel2.txt", "nivel3.txt" };
    int num_niveles = 3;
    int n;

    /* ─── Loop de niveles ─── */
    for (n = 0; n < num_niveles; n++) {

        /* Inicializar GameState para este nivel */
        gs.mapa.rows      = MAP_ROWS;
        gs.mapa.cols      = MAP_COLS;
        gs.mapa.celdas    = NULL;
        gs.tiene_llave    = 0;
        gs.monedas_recogidas = 0;
        gs.pasos          = 0;
        gs.nivel          = n + 1;

        /* Cargar mapa desde archivo */
        if (!cargar_mapa(archivos[n], &gs.mapa, &gs.jugador)) {
            printf("Error al cargar %s\n", archivos[n]);
            return 1;
        }

        /* NASM: contar monedas totales del nivel */
        gs.monedas_totales = contar_caracteres(gs.mapa.celdas,MAP_ROWS * MAP_COLS,SYM_COIN);

        /* NASM: contar celdas libres */
        gs.celdas_libres = contar_celdas_libres(gs.mapa.celdas, MAP_ROWS * MAP_COLS);

        printf("  Celdas libres en nivel %d: %d\n", gs.nivel, gs.celdas_libres);
        printf("  Monedas en nivel %d: %d\n", gs.nivel, gs.monedas_totales);
        printf("  Presiona cualquier tecla para iniciar...\n");
        _getch();

        /*Loop del juego*/
        while (1) {

            limpiar_pantalla();
            imprimir_ventana(&gs);
            mostrar_hud(&gs);

            tecla = _getch();
            resultado = procesar_tecla(tecla, &gs);

            if (resultado == -1) {
                /* Jugador presionó Q */
                printf("Juego terminado.\n");
                liberar_mapa(&gs.mapa);
                return 0;
            }

            if (resultado == 2) {
                /* Nivel completado */
                limpiar_pantalla();
                imprimir_ventana(&gs);
                mostrar_resumen_nivel(&gs);
                break;
            }
        }

        /* Acumular estadísticas globales */
        mon_posibles_total  += gs.monedas_totales;
        mon_recogidas_total += gs.monedas_recogidas;
        pasos_total         += gs.pasos;

        liberar_mapa(&gs.mapa);
    }

    /*Puntaje y resumen final*/
    puntaje = calcular_puntaje(mon_recogidas_total, pasos_total, num_niveles);

    limpiar_pantalla();
    mostrar_resumen_final(mon_recogidas_total, mon_posibles_total, pasos_total, num_niveles, puntaje);

    return 0;
}