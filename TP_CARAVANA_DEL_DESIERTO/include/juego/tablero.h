#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../tda/lista_circular_doble.h"


///PARA GENERAR EL TABLERO Y POSICIONAR AL JUGADOR
///Y A LOS N BANDIDOS, TABLERO.H DEBE CONOCER DICHAS ESTRUCUTAS
///POR ESO SE INCLUYEN EN TABLERO.H
#include "../entidad/bandido.h"
#include "../entidad/jugador.h"

/// EN ESTE MÓDULO DEBERÍA DESARROLLARSE UNICAMENTE GENERACIÓN/ACTUALIZACIÓN Y DESCTRUCCIÓN DEL TABLERO

//ESTRUCTURA DEL ARCHIVO CONFIG.TXT
typedef struct{
    int cantidadPosiciones;
    int vidas_inicio;
    int max_bandidos;
    int max_premios;
    int max_vidas_extras;
    int max_oasis;
    int max_tormentas;
}tConfig;

//REPRESENTACIÓN DE QUÉ HAY EN CADA NODO/casillero
//existe una forma mejor? seguro

typedef struct{
    char tipoBase; // '.', 'P', 'O', 'T', 'V', 'I', 'S'
    int hayJugador;   // 0 o 1
    int cantBandidos; // puede haber más de uno
}tCasillero;

#endif // TABLERO_H_INCLUDED
