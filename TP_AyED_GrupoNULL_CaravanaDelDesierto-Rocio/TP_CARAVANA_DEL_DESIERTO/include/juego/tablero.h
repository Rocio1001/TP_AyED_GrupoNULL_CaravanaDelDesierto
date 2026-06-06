#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINEA                256
#define CONFIG_OK                1
#define CONFIG_ERROR            -1
#define MAX_HOSTILIDAD_PERMITIDA 0.55
#define TABLERO_GENERADO         1
#define TABLERO_ERROR           -2
#define MAX_BANDIDOS_POR_CASILLERO  2
#define MAX_ELEMENTOS_ESTATICOS     2

#define RUTA_CONFIG   "config.txt"
#define RUTA_CARAVANA "caravana.txt"
#define RUTA_LOG      "log_movimientos.txt"

#include "../tda/lista_circular_doble.h"
#include "../entidad/bandido.h"
#include "../entidad/jugador.h"


// --- Config leida desde config.txt ---
typedef struct{
    int cantidadPosiciones;
    int vidasInicio;
    int maxBandidos;
    int maxPremios;
    int maxVidasExtras;
    int maxOasis;
    int maxTormentas;
}tConfig;

typedef struct{
    tListaDC lista;
    int cantidadCasilleros;
}tTablero;

// Tipos posibles de elemento estático en un casillero
typedef enum{
    CAS_VACIO,
    CAS_OASIS,
    CAS_TORMENTA,
    CAS_PREMIO,
    CAS_VIDA_EXTRA,
    CAS_INICIO,
    CAS_FIN
}tTipoCasillero;

// Cada nodo del tablero: hasta MAX_ELEMENTOS_ESTATICOS tipos estáticos,
// hasta MAX_BANDIDOS_POR_CASILLERO bandidos, y el jugador (0 o 1).
typedef struct{
    tTipoCasillero estaticos[MAX_ELEMENTOS_ESTATICOS];
    int cantEstaticos;
    int hayJugador;
    int cantBandidos;
}tCasillero;


// --- Creacion y destruccion ---
void crear_tablero(tTablero *tablero);
void destruir_tablero(tTablero *tablero);

// --- Configuracion ---
int leer_configuracion(const char* ruta_archivo, tConfig* config);
int validar_configuracion(const tConfig* config);
int preparar_configuracion(const char* ruta_archivo, tConfig* config);

// --- Consultas y modificacion de casilleros ---
int  casillero_es_inicio(const tCasillero* cas);
int  casillero_es_fin(const tCasillero* cas);
int  puede_agregar_estatico(const tCasillero* cas, tTipoCasillero tipo);
void remover_estatico_casillero(tCasillero* cas, tTipoCasillero tipo);
void reposicionar_bandido(tTablero* tablero, tBandido* bandido);

// --- Construccion del tablero ---
int construir_casilleros_estaticos_tablero(tTablero* tablero, const tConfig* config);
void posicionar_e_inicializar_jugador(tTablero* tablero, tJugador *jugador, const tConfig *config);
int posicionar_bandidos(tTablero* tablero, int totalBandidos, tBandido *bandido);
int construir_tablero(tTablero *tablero, tBandido **bandido, tJugador *jugador, const tConfig *config);

// --- Visualizacion ---
void actualizar_caravana_txt(tTablero *tablero, const char *pathCaravanaTxt);
void mostrar_caravana_txt(const char *pathCaravanaTxt);


#endif // TABLERO_H_INCLUDED
