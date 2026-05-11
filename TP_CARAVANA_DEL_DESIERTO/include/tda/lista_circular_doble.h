#ifndef LISTA_CIRCULAR_DOBLE_H_INCLUDED
#define LISTA_CIRCULAR_DOBLE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sNodoD{
    void* info;
    size_t tamInfo;
    struct sNodoD* ant;
    struct sNodoD* sig;
}tNodoD;

typedef tNodoD* tListaD;


#endif // LISTA_CIRCULAR_DOBLE_H_INCLUDED
