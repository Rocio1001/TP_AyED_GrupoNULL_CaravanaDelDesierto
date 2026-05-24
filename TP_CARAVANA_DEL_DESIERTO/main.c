#include <stdio.h>
#include <stdlib.h>

#define RUTA_CONFIG "config.txt"
#define RUTA_CARAVANA "caravana.txt"
#include "include/juego/partida.h"
#define RUTA

int main(){
    //tPartida miPartida;
    ///PRUEBA DE GENERAR UN TABLERO.
    srand(time(NULL));
    tTablero tablero;
    tBandido *bandidos;
    tJugador jugador;
    crear_tablero(&tablero);
    if(construir_tablero(&tablero,&bandidos,&jugador,RUTA_CONFIG) != TABLERO_GENERADO){
        destruir_tablero(&tablero);
        return 0;
    }
    actualizar_caravana_txt(&tablero,RUTA_CARAVANA);
    mostrar_caravana_txt(RUTA_CARAVANA);
    system("pause");
    destruir_tablero(&tablero);
    return 0;
}
