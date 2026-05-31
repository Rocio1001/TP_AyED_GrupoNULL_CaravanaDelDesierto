#ifndef RANKING_H_INCLUDED
#define RANKING_H_INCLUDED

#include "../tda/lista_simple.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define ERROR_ARCH -1

typedef struct{
    int idJugador;
    char jugador[30];
    int totalPuntos;
    int totalMovimientos;
    int partidasJugadas;
}tRanking;

///creacion, visualización y destrucción del ranking
void crear_y_mostrar_ranking(const char *pathHistorico);

///cmp de inserción
int cmp_id(const void* idA, const void* idB);
///cmp de ordenamiento
int cmp_criterio_ranking(const void* a, const void* b);
///accion al insertar
void acumular_ranking(void** datoEnLista, size_t* tamDato, const void* datoAAcumular, size_t tamDatoAcumular);
///acción al recorrer ranking
void imprimir_ranking(const void* dato, size_t tamDato, void* params);



#endif // RANKING_H_INCLUDED
