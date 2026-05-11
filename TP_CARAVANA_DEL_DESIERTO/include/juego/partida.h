#ifndef PARTIDA_H_INCLUDED
#define PARTIDA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "reglas.h"
#include "movimiento.h"

///SUJETO A CAMBIOS



typedef struct{
    int id;
    char nombreInterno[30];
    char nombre[30];
    int vidas;
    int proteccion;
    int turno_valido;  // 1(puede moverse) 0 (no puede moverse) - puede pasar que pierda el turno por alguna razon
    int puntos;
    tNodoD* posicion;
}tJugador;

typedef struct{
    int id;
    tNodoD* posicion;
}tBandido;

/*
typedef struct{
    int id;
    int nroMov;
    char descripcion[3]; //FX o BX o NT(en caso de que haya sido un movimiento bloqueado)
}tLogMovimientos;*/


///        estoy pensando en modularizar este .h y separar algunas estructuras más
///        ponerlas en otro .h y sus respectivos funcionamientos

//  el tipo de dato de retorno de la función, puede variar, pero por ahora tiro esto
void mover_jugador(tJugador *jugador);
void aplicar_accion_jugador();





#endif // PARTIDA_H_INCLUDED
