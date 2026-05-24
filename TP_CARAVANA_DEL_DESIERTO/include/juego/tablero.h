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

/// EN ESTE M�DULO DEBER�A DESARROLLARSE UNICAMENTE GENERACI�N/ACTUALIZACI�N Y DESCTRUCCI�N DEL TABLERO

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

//REPRESENTACI�N DE QU� HAY EN CADA NODO/casillero
//existe una forma mejor? seguro
typedef enum{
    CAS_VACIO,
    CAS_OASIS,
    CAS_TORMENTA,
    CAS_VIDA_EXTRA,
    CAS_INICIO,
    CAS_FIN
}tTipoCasillero;
typedef struct{
    tTipoCasillero tipo;// '.', 'P', 'O', 'T', 'V', 'I', 'S'
    int hayJugador; // 0 o 1
    int cantBandidos; // puede haber m�s de uno
}tCasillero;
#endif // TABLERO_H_INCLUDED
