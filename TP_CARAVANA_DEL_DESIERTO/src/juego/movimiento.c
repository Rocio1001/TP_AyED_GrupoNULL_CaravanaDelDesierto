#include "../../include/juego/movimiento.h"

/* =========================================================
   VALIDACION DE TURNO
   ========================================================= */

int turno_valido(const tJugador* jugador){
    return jugador->turnoValido;
}

/* =========================================================
   DADO Y ELECCION DE DIRECCION
   ========================================================= */

int tirar_dado(void){
    int valor;
    printf("\n  Presione Enter para tirar el dado...");
    while(getchar() != '\n');
    valor = (rand() % 6) + 1;
    printf("  Resultado del dado: %d\n", valor);
    return valor;
}

int elegir_direccion(void){
    char buf[16];
    int dir = -1;
    do {
        printf("  Direccion: 1 = Adelante   0 = Atras: ");
        fgets(buf, sizeof(buf), stdin);
        dir = atoi(buf);
    } while(dir != 0 && dir != 1);
    return dir;
}

/* =========================================================
   HELPERS INTERNOS
   ========================================================= */

/*
 * Devuelve el nodo que se encuentra `pasos` posiciones desde `origen`
 * en la direccion dada (circular, sin rebote).
 */
static tNodoD* calcular_destino(tNodoD* origen, int pasos, int dir){
    tNodoD* pos = origen;
    int i;
    for(i = 0; i < pasos; i++)
        pos = (dir == 1) ? pos->sig : pos->ant;
    return pos;
}

/*
 * Calcula la direccion optima para que el bandido siga al jugador
 * eligiendo el camino mas corto (el tablero es circular).
 * Retorna 1 (adelante) o -1 (atras).
 */
static int direccion_optima_bandido(tNodoD* posBandido, tNodoD* posJugador, int total){
    tNodoD* act = posBandido;
    int dist_ade = 0;
    while(act != posJugador && dist_ade < total){
        act = act->sig;
        dist_ade++;
    }
    return (dist_ade <= total - dist_ade) ? 1 : -1;
}

/* =========================================================
   MOVIMIENTO DEL JUGADOR (con rebote en INICIO y FIN)
   ========================================================= */

static void mover_jugador(tJugador* jugador, tTablero* tablero, int pasos, int dir, int* gano){
    tCasillero* cas;
    tNodoD* pos;
    int i, direccion;

    pos = jugador->posicion;
    direccion = (dir == 1) ? 1 : -1;

    cas = (tCasillero*)pos->info;
    cas->hayJugador = 0;

    for(i = 0; i < pasos; i++){
        pos = (direccion == 1) ? pos->sig : pos->ant;
        cas = (tCasillero*)pos->info;

        if(casillero_es_fin(cas) && direccion == 1){
            if(i == pasos - 1){
                *gano = 1;  // llegada exacta al FIN -> victoria
            } else {
                direccion = -1; // rebote en FIN
            }
        }
        /* Sin rebote en INICIO: partida.c garantiza que el jugador
           nunca retrocede mas pasos de los disponibles. */
    }

    cas = (tCasillero*)pos->info;
    cas->hayJugador = 1;
    jugador->posicion = pos;
}

/* =========================================================
   MOVIMIENTO DEL BANDIDO (circular, sin rebote)
   El conteo de bandidos por casillero se actualiza al ENCOLAR,
   no al desencolar, para que las validaciones posteriores sean
   correctas y no superen MAX_BANDIDOS_POR_CASILLERO.
   ========================================================= */

static void mover_bandido(tBandido* bandido, int pasos, int dir){
    int i;
    /* Los conteos ya fueron actualizados al encolar; solo mueve el puntero */
    for(i = 0; i < pasos; i++) {
        bandido->posicion = (dir == 1) ? bandido->posicion->sig : bandido->posicion->ant;
    }
}

/* =========================================================
   COLA DE MOVIMIENTOS - JUGADOR
   ========================================================= */

void encolar_movimiento(tCola* cola, tJugador* jugador, int pasos, int direccion){
    tMovimiento mov;
    mov.tipo      = TIPO_JUGADOR;
    mov.entidad   = jugador;
    mov.pasos     = pasos;
    mov.direccion = (direccion == 1) ? 1 : -1;
    insertar_en_cola(cola, &mov, sizeof(tMovimiento));
}

/*
 * Desencola el proximo movimiento y lo aplica.
 * Retorna TODO_OK si habia algo, COLA_VACIA si no.
 */
int desencolar_y_mover(tCola* cola, tTablero* tablero, int* gano){
    tMovimiento mov;
    if(sacar_de_cola(cola, &mov, sizeof(tMovimiento)) == COLA_VACIA)
        return COLA_VACIA;

    if(mov.tipo == TIPO_JUGADOR){
        mover_jugador((tJugador*)mov.entidad, tablero, mov.pasos, mov.direccion, gano);
    } else if(mov.tipo == TIPO_BANDIDO){
        mover_bandido((tBandido*)mov.entidad, mov.pasos, mov.direccion);
    }

    return TODO_OK;
}

/* =========================================================
   COLA DE MOVIMIENTOS - BANDIDOS
   ========================================================= */

/*
 * Procesa el turno de todos los bandidos:
 *   1. Cada bandido tira el dado.
 *   2. Calcula la direccion optima para acercarse al jugador.
 *   3. Valida que el casillero destino no supere MAX_BANDIDOS_POR_CASILLERO.
 *      Si esta lleno, vuelve a tirar (maximo 20 intentos).
 *   4. Actualiza cantBandidos inmediatamente (origen-- / destino++).
 *      De esta forma, el siguiente bandido ve el conteo ya reservado
 *      y no puede elegir un casillero que ya alcanzó el maximo.
 *   5. Encola el movimiento (solo el puntero se mueve al desencolar).
 */
void procesar_turno_bandidos(tCola* cola,
                              tBandido* bandidos, int cantBandidos,
                              tJugador* jugador, tTablero* tablero){
    int i, dado, dir, intentos;
    tNodoD* destino;
    tCasillero* casOrigen;
    tCasillero* casDest;
    tMovimiento mov;

    for(i = 0; i < cantBandidos; i++){
        if(bandidos[i].posicion == NULL) continue; // bandido eliminado

        intentos = 0;
        do {
            dado    = (rand() % 6) + 1;
            dir     = direccion_optima_bandido(bandidos[i].posicion,
                                               jugador->posicion,
                                               tablero->cantidadCasilleros);
            destino = calcular_destino(bandidos[i].posicion, dado, dir);
            casDest = (tCasillero*)destino->info;
            intentos++;
        } while(casDest->cantBandidos >= MAX_BANDIDOS_POR_CASILLERO && intentos < 20);

        /* Reservar el slot: actualizar conteos antes de encolar */
        casOrigen = (tCasillero*)bandidos[i].posicion->info;
        casOrigen->cantBandidos--;
        casDest->cantBandidos++;

        mov.tipo      = TIPO_BANDIDO;
        mov.entidad   = &bandidos[i];
        mov.pasos     = dado;
        mov.direccion = dir;
        insertar_en_cola(cola, &mov, sizeof(tMovimiento));
    }
}
