#ifndef MOVIMIENTO_H_INCLUDED
#define MOVIMIENTO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tablero.h"
#include "../tda/cola_dinamica.h"

// Tipos de entidad que pueden moverse
typedef enum{
    TIPO_JUGADOR,
    TIPO_BANDIDO
}tTipoEntidad;

// Movimiento que se encola antes de aplicarse
typedef struct{
    tTipoEntidad tipo;
    void* entidad;
    int pasos;
    int direccion; // 1 = adelante, -1 = atras
}tMovimiento;

// Registro de un movimiento para el log de la partida
typedef struct{
    int id;
    int nroMov;
    char descripcion[3]; // "FX", "BX" o "NT"
}tLogMovimientos;


// --- Validacion de turno ---
int turno_valido(const tJugador* jugador);

// --- Dado y direccion ---
int tirar_dado(void);
int elegir_direccion(void);

// --- Cola: jugador ---
void encolar_movimiento(tCola* cola, tJugador* jugador, int pasos, int direccion);
int  desencolar_y_mover(tCola* cola, tTablero* tablero, int* gano);

// --- Cola: bandidos ---
void procesar_turno_bandidos(tCola* cola,
                              tBandido* bandidos, int cantBandidos,
                              tJugador* jugador, tTablero* tablero);

// --- Efectos de casillero ---
void aplicar_efectos_casillero(tJugador* jugador, tCasillero* cas);
void verificar_encuentro_bandidos(tJugador* jugador, tBandido* bandidos,
                                   int cantBandidos, tTablero* tablero);

// --- Helpers internos de partida ---
int  distancia_desde_inicio(const tTablero* tablero, const tJugador* jugador);
void mostrar_estado(int numero_turno, const tJugador* jugador, tTablero* tablero);
void escribir_log(FILE* logFile, const tLogMovimientos* entrada);
void mostrar_log_partida(const char* ruta);


#endif // MOVIMIENTO_H_INCLUDED
