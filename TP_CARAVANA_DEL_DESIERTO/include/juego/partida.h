#ifndef PARTIDA_H_INCLUDED
#define PARTIDA_H_INCLUDED

#include "tablero.h"
#include "../tda/cola_dinamica.h"

///SUJETO A CAMBIOS


typedef enum{
     TIPO_JUGADOR,
     TIPO_BANDIDO
}tTipoEntidad;

//movimiento a encolar
typedef struct{
    tTipoEntidad tipo; //jugador o bandido
    void* entidad; //puntero a jugador o bandido
    int pasos; //cantidad que se movió la entidad (lo que sale ens el dado)
    int direccion; // 1 (avanzo), -1(retrocedo)
}tMovimiento;



typedef struct{
    int id;
    char nombreInterno[30];
    char nombre[30];
    int vidas;
    int proteccion;
    int turno_valido;  // 1(puede moverse) 0 (no puede moverse) - puede pasar que pierda el turno por alguna razon
    int cant_movimientos; //lo que tiro el dado
    int puntos;
    tNodo* posicion;
}tJugador;



typedef struct{
    int id;
    int nroMov;
    char descripcion[3]; //FX o BX o NT(en caso de que haya sido un movimiento bloqueado)
}tLogMovimientos;


///        estoy pensando en modularizar este .h y separar algunas estructuras más
///        ponerlas en otro .h y sus respectivos funcionamientos

//  el tipo de dato de retorno de la función, puede variar, pero por ahora tiro esto
void mover_jugador(tJugador *jugador);
void aplicar_accion_jugador();





#endif // PARTIDA_H_INCLUDED
