#ifndef LISTA_CIRCULAR_DOBLE_H_INCLUDED
#define LISTA_CIRCULAR_DOBLE_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TODO_OK 1
#define LISTA_LLENA -1
#define LISTA_VACIA -2
#define DUPLICADO -3
#define MIN(X,Y)((X)<(Y)?(X):(Y))


typedef struct sNodoD{
    void* info;
    size_t tamInfo;
    struct sNodoD* ant;
    struct sNodoD* sig;
}tNodoD;

typedef tNodoD *tListaDC;

/*
    Esta implementación de Lista Doble Circular, en la inserción y eliminación de nodos, el puntero a la lista doble SIEMPRE apunta al primer nodo insertado.
    Si bien, ese primer nodo apunta al último nodo, el puntero a la lista siempre apunta al principio de la lista.

    Si se inserta al final, dicho puntero no se modifica.
    Si se inserta al inicio, el puntero se modifica para apuntar al nuevo primero.
*/


///operaciones básicas
void crear_lista_doble_cir(tListaDC *pl);
void vaciar_lista_doble_cir(tListaDC *pl);



///inserción
int insertar_pri_lista_doble_cir(tListaDC *pl,const void *pd, size_t tam);
int insertar_ult_lista_doble_cir(tListaDC *pl,const void *pd, size_t tam);

///eliminar
int sacar_pri_lista_doble_cir(tListaDC *pl, void *pd, size_t tam);
int sacar_ult_lista_doble_cir(tListaDC *pl, void *pd, size_t tam);


///ver en lista
int ver_pri_lista_doble_cir(const tListaDC* pl, void *pd, size_t tam);
int ver_ult_lista_doble_cir(const tListaDC* pl, void *pd, size_t tam);


#endif // LISTA_CIRCULAR_DOBLE_H_INCLUDED
