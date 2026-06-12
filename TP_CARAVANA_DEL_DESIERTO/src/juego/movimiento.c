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

/* =========================================================
   EFECTOS DE CASILLERO
   ========================================================= */

/*
 * Aplica los efectos de los elementos estaticos del casillero sobre el jugador.
 * Premio y Vida Extra desaparecen del tablero al ser recogidos.
 * Oasis otorga proteccion para el SIGUIENTE turno.
 * Tormenta hace perder el SIGUIENTE turno (o consume la proteccion si la tenia).
 */
void aplicar_efectos_casillero(tJugador* jugador, tCasillero* cas){
    int i;
    for(i = 0; i < cas->cantEstaticos; ){
        switch(cas->estaticos[i]){
            case CAS_PREMIO:
                jugador->puntos += 100;
                printf("  Premio! +100 puntos. Total: %d pts.\n", jugador->puntos);
                remover_estatico_casillero(cas, CAS_PREMIO);
                break;

            case CAS_VIDA_EXTRA:
                jugador->vidas++;
                printf("  Vida extra! Vidas: %d\n", jugador->vidas);
                remover_estatico_casillero(cas, CAS_VIDA_EXTRA);
                break;

            case CAS_OASIS:
                jugador->proteccion = 2;
                printf("  Oasis! Tendras proteccion en el proximo turno.\n");
                i++;
                break;

            case CAS_TORMENTA:
                if(jugador->proteccion){
                    jugador->proteccion = 0;
                    printf("  Tormenta! Tu proteccion te salvo.\n");
                } else {
                    jugador->turnoValido = 0;
                    printf("  Tormenta! Perderas el proximo turno.\n");
                }
                i++;
                break;

            default:
                i++;
                break;
        }
    }
}

/*
 * Verifica si el jugador comparte casillero con algun bandido activo.
 * Si lo hay:
 *   - El bandido es eliminado del tablero.
 *   - Si el jugador tiene proteccion: la consume y NO pierde vida ni vuelve al inicio.
 *   - Si no tiene proteccion: pierde 1 vida y vuelve al INICIO.
 *     Al llegar al inicio se reposicionan los bandidos que hubiera alli.
 */
void verificar_encuentro_bandidos(tJugador* jugador, tBandido* bandidos,
                                   int cantBandidos, tTablero* tablero){
    tCasillero* cas = (tCasillero*)jugador->posicion->info;
    tCasillero* casInicio;
    int i, j;

    if(cas->cantBandidos == 0) return;

    for(i = 0; i < cantBandidos; i++){
        if(bandidos[i].posicion == jugador->posicion){

            cas->cantBandidos--;
            bandidos[i].posicion = NULL;

            if(jugador->proteccion){
                jugador->proteccion = 0;
                printf("  Un bandido te intercepto! Tu proteccion te salvo (sin perdida de vida).\n");
            } else {
                jugador->vidas--;
                printf("  Un bandido te intercepto! Perdiste 1 vida. Vidas: %d\n",
                       jugador->vidas);

                cas->hayJugador = 0;
                jugador->posicion = tablero->lista;
                casInicio = (tCasillero*)jugador->posicion->info;
                casInicio->hayJugador = 1;
                printf("  Volviste al inicio.\n");

                if(casInicio->cantBandidos > 0){
                    printf("  Habia bandidos en el inicio. Seran reposicionados.\n");
                    for(j = 0; j < cantBandidos; j++){
                        if(bandidos[j].posicion == tablero->lista)
                            reposicionar_bandido(tablero, &bandidos[j]);
                    }
                }
            }
            break;
        }
    }
}

/* =========================================================
   HELPERS INTERNOS DE PARTIDA
   ========================================================= */

/*
 * Cuenta cuantos casilleros hay entre INICIO y la posicion del jugador
 * avanzando hacia adelante.
 */
int distancia_desde_inicio(const tTablero* tablero, const tJugador* jugador){
    tNodoD* act = tablero->lista;
    int dist = 0;
    while(act != jugador->posicion && dist < tablero->cantidadCasilleros){
        act = act->sig;
        dist++;
    }
    return dist;
}

void mostrar_estado(int numero_turno, const tJugador* jugador, tTablero* tablero){
    actualizar_caravana_txt(tablero, RUTA_CARAVANA);
    mostrar_caravana_txt(RUTA_CARAVANA);
    printf("\n  Turno: %-4d | Jugador: %-20s | Vidas: %d | Puntos: %-6d | Proteccion: %s\n",
           numero_turno + 1,
           jugador->nombre,
           jugador->vidas,
           jugador->puntos,
           jugador->proteccion > 0 ? "Si" : "No");
    printf("  -------------------------------------------------------\n");
}

void escribir_log(FILE* logFile, const tLogMovimientos* entrada){
    if(!logFile) return;
    fprintf(logFile, "ID: %-4d | Turno: %-4d | Movimiento: %s\n",
            entrada->id, entrada->nroMov, entrada->descripcion);
}

void mostrar_log_partida(const char* ruta){
    FILE* f = fopen(ruta, "r");
    char linea[128];
    if(!f){ printf("\n  (Log no disponible)\n"); return; }
    printf("\n  +-----------------------------------------------+\n");
    printf("  |         LOG DE MOVIMIENTOS DE LA PARTIDA      |\n");
    printf("  +-----------------------------------------------+\n");
    while(fgets(linea, sizeof(linea), f))
        printf("  %s", linea);
    printf("  +-----------------------------------------------+\n");
    fclose(f);
}
