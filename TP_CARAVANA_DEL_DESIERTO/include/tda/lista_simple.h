#ifndef LISTA_SIMPLE_H_INCLUDED
#define LISTA_SIMPLE_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TODO_OK 1
#define LISTA_LLENA -1
#define LISTA_VACIA -2
#define DUPLICADO -3
#define MIN(X,Y)((X)<(Y)?(X):(Y))

///******************************************************************************************
///  PRIMITIVAS  MINIMAS Y NECESARIAS PARA HACER FUNCIONAR EL RANKING
///******************************************************************************************


typedef struct sNodo{
    void* info;
    size_t tamInfo;
    struct sNodo* sig;
}tNodo;

typedef tNodo* tLista;

///operaciones básicas
void crear_lista(tLista* pl);
void vaciar_lista (tLista* pl);
///insertar ordenado
int insertar_ordenado_lista(tLista* pl, const void* pd, size_t tam,int aceptDup, int(*cmp)(const void*, const void*),void(*accion)(void**,size_t*,const void*,size_t));
///ordenar
//primitiva de ordenamiento que me enseñaron el cuatri pasado, evaluar si esta es la mejor forma de ordenar o si queremos elegir otro algoritmo de ordenamiento
void ordenar_lista(tLista *pl, int (*cmp)(const void*, const void*));
///recorrer lista
void recorrer_lista(tLista * pl, void (*accion)(const void*,size_t, void*), void* param);

#endif // LISTA_SIMPLE_H_INCLUDED
