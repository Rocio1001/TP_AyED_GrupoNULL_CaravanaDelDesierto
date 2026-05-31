#ifndef MOVIMIENTO_H_INCLUDED
#define MOVIMIENTO_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
///como tablero incluye a jugador.h y bandido.h, podemos acceder a ellos solo incluyendo tablero.h
#include "tablero.h"

///#include "../tda/cola_dinamica.h"
/*
    dejo comentado el include "tda_cola" dado que me tira error por re-definición de tNodo, dado que en el main incluyo al ranking.h, el cuál incluye a lista.h y ahí, se define tNodo,
    generando una re-definición del mismo en el main

    cuando desarrollemos este modulo, analizar y decidir cómo manejarlo.

*/
typedef enum{
     TIPO_JUGADOR,
     TIPO_BANDIDO
}tTipoEntidad;

//movimiento a encolar
typedef struct{
    tTipoEntidad tipo; //jugador o bandido
    void* entidad; //puntero a jugador o bandido
    int pasos; //cantidad que se movi� la entidad (lo que sale ens el dado)
    int direccion; // 1 (avanzo), -1(retrocedo)
}tMovimiento;

//tipo de dato que va a contener el log de movimientos de la partida
typedef struct{
    int id;
    int nroMov;
    char descripcion[3]; //FX o BX o NT(en caso de que haya sido un movimiento bloqueado)
}tLogMovimientos;


#endif // MOVIMIENTO_H_INCLUDED
