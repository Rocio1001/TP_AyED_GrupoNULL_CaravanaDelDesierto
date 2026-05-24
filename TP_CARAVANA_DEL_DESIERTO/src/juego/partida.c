#include "../../include/juego/partida.h"


int inicializar_partida(tPartida* partida,tBandido **bandido, const char* rutaConfig){ //llama a crear tablero caravana
    ///creo el tablero (vacio)
    crear_tablero(&partida->tablero);
    construir_tablero(&partida->tablero, bandido,&partida->jugador,rutaConfig);
    return TODO_OK;
}
int ejecutar_partida(tPartida* partida){// aca se juega, se ejecutan funciones de movimientos.h
    //desarrollo de los movimientos
    return TODO_OK;
}

void finalizar_partida(tPartida* partida){//llama a destruir tablero
    destruir_tablero(&partida->tablero);
}
