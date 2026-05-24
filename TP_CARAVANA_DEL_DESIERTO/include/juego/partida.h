#ifndef PARTIDA_H_INCLUDED
#define PARTIDA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "movimiento.h"


//FALTA DESARROLLAR Y ANALIZAR MÁS
/*
Lo pensado es:
El módulo partida.h es el Orquestador. 
Su único trabajo es ser el Director de Orquesta: decirle al tablero que se arme, decirle al jugador que tire el dado, 
decirle a los bandidos que se muevan, y avisar cuándo alguien ganó o perdió, etc
*/


typedef struct {
    tTablero tablero;
    tJugador jugador;
    int estado_juego; // 0 = Jugando, 1 = Gan�, -1 = Perdi�
    int numero_turno;
} tPartida;

int inicializar_partida(tPartida* partida,tBandido **bandido, const char* rutaConfig); //llama a construir tablero caravana
int ejecutar_partida(tPartida* partida); // ac� se juega, se ejecutan funciones de movimientos.h

//es necesario? para mi es redundante. Y que llame directo al desarrollado en tablero.c o traer la función a este modulo (analizar)
void actualizar_vista_juego(tPartida* partida);//llama a actualizar tablero 

void finalizar_partida(tPartida* partida);//llama a destruir tablero




#endif // PARTIDA_H_INCLUDED
