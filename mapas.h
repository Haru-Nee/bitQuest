#ifndef MAPAS_H
#define MAPAS_H

#include "juego.h"

/*Carga un mapa desde un .txt
retorna 1 si, 0 si error*/
int cargar_mapa(const char *nombre, Mapa *mapa, Posicion *pos_jugador);

/*Libera la memoria del mapa*/
void liberar_mapa(Mapa *mapa);
#endif