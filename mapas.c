//Cargar y liberar los mapas de memoria

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "juego.h"
#include "mapas.h"

int cargar_mapa(const char *nombre, Mapa *mapa, Posicion *pos_jugador) {

    FILE *archivo;
    int i = 0;
    int j = 0;
    int c;

    archivo = fopen(nombre, "r");
    if (archivo == NULL) {
        return 0;
    }

    mapa->celdas = (char*) malloc(mapa->rows * mapa->cols * sizeof(char));
    if (mapa->celdas == NULL) {
        fclose(archivo);
        return 0;
    }

    // Inicializar todo con '.'
    memset(mapa->celdas, '.', mapa->rows * mapa->cols);
    while ((c = fgetc(archivo)) != EOF && i < mapa->rows) {
        // Ignorar saltos de línea
        if (c == '\n' || c == '\r') {
            continue;
        }

        mapa->celdas[i * mapa->cols + j] = (char)c;

        // Detectar jugador
        if ((char)c == 'P') {
            pos_jugador->fila = i;
            pos_jugador->col = j;
        }
        j++;

        if (j >= mapa->cols) {
            j = 0;
            i++;
        }
    }

    fclose(archivo);
    return 1;
}


void liberar_mapa(Mapa *mapa) {
    if (mapa->celdas != NULL) {
        free(mapa->celdas);
        mapa->celdas = NULL;
    }
}

