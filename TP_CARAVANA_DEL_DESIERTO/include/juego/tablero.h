#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINEA 256
#define CONFIG_OK 1
#define CONFIG_ERROR -1
#define MAX_HOSTILIDAD_PERMITIDA 0.55 // 55% máximo de casilleros malos
#define TABLERO_GENERADO 1
#define TABLERO_ERROR -2
#define MAX_BANDIDOS_POR_CASILLERO 2

#include "../tda/lista_circular_doble.h"

/// cambios
#include "../tda/cola_dinamica.h"


///PARA GENERAR EL TABLERO Y POSICIONAR AL JUGADOR Y A LOS N BANDIDOS, TABLERO.H DEBE CONOCER DICHAS ESTRUCUTAS
///POR ESO SE INCLUYEN EN TABLERO.H
#include "../entidad/bandido.h"
#include "../entidad/jugador.h"

/// EN ESTE M�DULO DEBER�A DESARROLLARSE UNICAMENTE GENERACI�N/ACTUALIZACI�N Y DESCTRUCCI�N DEL TABLERO


//ESTRUCTURA DEL ARCHIVO CONFIG.TXT
typedef struct{
    int cantidadPosiciones;
    int vidasInicio;
    int maxBandidos;
    int maxPremios;
    int maxVidasExtras;
    int maxOasis;
    int maxTormentas;
}tConfig;

typedef struct sTablero{
    tListaDC lista;
    int cantidadCasilleros;
}tTablero;

//REPRESENTACI�N DE QU� HAY EN CADA NODO/casillero
//existe una forma mejor? seguro
typedef enum{
    CAS_VACIO,
    CAS_OASIS,
    CAS_TORMENTA,
    CAS_PREMIO,
    CAS_VIDA_EXTRA,
    CAS_INICIO,
    CAS_FIN
}tTipoCasillero;
typedef struct{
    tTipoCasillero tipo;// '.', 'P', 'O', 'T', 'V', 'I', 'S'
    int hayJugador; // 0 o 1
    int cantBandidos; // puede haber m�s de uno
}tCasillero;



///creacion y destrucción
void crear_tablero(tTablero *tablero);
void destruir_tablero(tTablero *tablero);

///validación de juego viable
int validar_configuracion(const tConfig* config);
int leer_configuracion(const char* ruta_archivo, tConfig* config);

///construyo y coloco los casilleros en el tablero
int construir_casilleros_estaticos_tablero(tTablero* tablero, const tConfig* config);

///posiciono a los bandidos y al jugador
void posicionar_yE_inicializar_jugador(tTablero* tablero, tJugador *jugador, tConfig *config);
int posicionar_bandidos(tTablero* tablero, int totalBandidos, tBandido *bandido);

///genero TODO (llama a las funciones que hacen posible la creación de todo el tablero)
int construir_tablero(tTablero *tablero, tBandido **bandido, tJugador *jugador, const char *pathConfigTxt);

///escribe caravana.txt
void actualizar_caravana_txt(tTablero *tablero, const char *pathCaravanaTxt);
void mostrar_caravana_txt(const char *pathCaravanaTxt);

/// cambios ==========================================================
void actualizar_tablero (tTablero *tablero, tCola* colaMovimientos);

///esta iria en movimientos.h y .c pero no andaba ahí
///Deje la declaracion aca en tablero.h pero el desarrollo en movimiento.c (ya se que no van asi, pero no lo puede hacer funcionar)
void aplicarMovimientos(tTablero* tablero, tCola* colaMovimientos);

#endif // TABLERO_H_INCLUDED
