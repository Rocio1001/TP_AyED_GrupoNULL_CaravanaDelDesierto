#ifndef LISTA_SIMPLE_H_INCLUDED
#define LISTA_SIMPLE_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "nodo.h"

#define TODO_OK       1
#define LISTA_LLENA  -1
#define LISTA_VACIA  -2
#define DUPLICADO    -3
#define NO_ENCONTRADO -4
#define POS_INVALIDA  -5
#define MIN(X,Y)((X)<(Y)?(X):(Y))

typedef tNodo* tLista;

///operaciones básicas
void crear_lista(tLista* pl);
void vaciar_lista(tLista* pl);
///recorrido
void recorrer_lista(tLista* pl, void (*accion)(const void*, size_t, void*), void* param);
///insercion ordenada (aceptDup=0: si hay duplicado segun cmp llama a accion en lugar de insertar)
int insertar_ordenado_lista(tLista* pl, const void* pd, size_t tam, int aceptDup,
                            int(*cmp)(const void*, const void*),
                            void(*accion)(void**, size_t*, const void*, size_t));
///reordenamiento in-place
void ordenar_lista(tLista* pl, int(*cmp)(const void*, const void*));

#endif // LISTA_SIMPLE_H_INCLUDED
