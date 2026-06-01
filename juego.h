#ifndef JUEGO_H
#define JUEGO_H
#define MAP_ROWS 60      //Renglones del mapa completo
#define MAP_COLS 60     //Columnas del mapa completo
#define VIEW_ROWS 20     //Renglones que el jugador vera
#define VIEW_COLS 20    //Columnas que el jugador vera
//Simbolos del mapa
#define SYM_WALL   '#'
#define SYM_FREE   '.'
#define SYM_PLAYER 'P'
#define SYM_COIN   'M'
#define SYM_KEY    'K'
#define SYM_DOOR   'D'
#define SYM_EXIT   'E'

/*Colores ANSI*/
#define COLOR_RESET   "\033[0m"
#define COLOR_PARED   "\033[90m"    /* gris oscuro  → # */
#define COLOR_LIBRE   "\033[37m"    /* blanco       → . */
#define COLOR_JUGADOR "\033[96m"    /* cyan brillante → P */
#define COLOR_MONEDA  "\033[93m"    /* amarillo     → M */
#define COLOR_LLAVE   "\033[33m"    /* naranja      → K */
#define COLOR_PUERTA  "\033[91m"    /* rojo         → D */
#define COLOR_SALIDA  "\033[92m"    /* verde        → E */

/* Color por nivel (fondo del HUD) */
#define COLOR_NIV1    "\033[34m"    /* azul   */
#define COLOR_NIV2    "\033[35m"    /* morado */
#define COLOR_NIV3    "\033[31m"    /* rojo   */

typedef struct {
    int fila;
    int col;
} Posicion;

typedef struct {
    int rows;
    int cols;
    char *celdas;
} Mapa;

typedef struct {
    Mapa     mapa;
    Posicion jugador;
    int      tiene_llave;
    int      monedas_recogidas;
    int      monedas_totales;
    int      celdas_libres;
    int      pasos;
    int      nivel;
} GameState;

/* Prototipos C */
void limpiar_pantalla(void);
void imprimir_ventana(const GameState *gs);
void mostrar_hud(const GameState *gs);
void mostrar_resumen_nivel(const GameState *gs);
void mostrar_resumen_final(int mon_total, int mon_posibles, int pasos_total, int niveles, int puntaje);
int  procesar_tecla(int tecla, GameState *gs);

/* Prototipos NASM */
extern int contar_caracteres(char *mapa, int total, char c);
extern int validar_movimiento(char *mapa, int cols, int fila, int col);
extern int calcular_puntaje(int monedas, int pasos, int niveles);
extern int detectar_objeto(char *mapa, int cols, int fila, int col, char obj);
extern int contar_celdas_libres(char *mapa, int total);
#endif