#ifndef HISTORICO_H_INCLUDED
#define HISTORICO_H_INCLUDED

#include <stdio.h>
#include <string.h>

typedef struct {
    int  idPartida;
    int  idJugador;
    char nombreJugador[30];
    int  puntos;
    int  movimientos;
} tHistorico;

void escribir_historico(tHistorico* historico, const char* pathHistorico);
void mostrar_historico(const char* file_name);

#endif // HISTORICO_H_INCLUDED
