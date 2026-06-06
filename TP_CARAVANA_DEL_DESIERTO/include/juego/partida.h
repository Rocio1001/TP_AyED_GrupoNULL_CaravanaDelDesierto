#ifndef PARTIDA_H_INCLUDED
#define PARTIDA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "movimiento.h"

/*
El módulo partida.h es el Orquestador.
Su único trabajo es ser el Director de Orquesta: decirle al tablero que se arme,
decirle al jugador que tire el dado, decirle a los bandidos que se muevan,
y avisar cuándo alguien ganó o perdió.
*/

typedef struct {
    tTablero  tablero;
    tJugador  jugador;
    tBandido* bandidos;     // vector de bandidos (alojado dinamicamente)
    int       cantBandidos;
    int       estado_juego; // 0 = Jugando, 1 = Gano, -1 = Perdio
    int       numero_turno;
} tPartida;

int  inicializar_partida(tPartida* partida, const char* rutaConfig);
int  ejecutar_partida(tPartida* partida);
void actualizar_vista_juego(tPartida* partida);
void finalizar_partida(tPartida* partida);

#endif // PARTIDA_H_INCLUDED
