#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CLA_DUP -2
#define SIN_MEM -4 ///CAMBIE ESTE POR -4
#define SIN_INICIALIZAR -3
#define ERROR_ARCH -1 ///Y ESTE POR -1 POR QUE ME DABA WARNING DE REDEFINICION
#define TODO_OK 1
#define MIN(X,Y) ((X)<(Y)?(X):(Y))
#define MAX(X,Y) ((X)>(Y)?(X):(Y))

typedef struct sNodoA{ ///le cambie el nombre de sNodo a sNodoA pq si no me daba error por nodo.h
    void* info;
    size_t tamInfo;
    struct sNodoA *izq;
    struct sNodoA *der;
}tNodoA;

typedef tNodoA *tArbol;

///CREACION
void crear_arbol(tArbol *pa);
///DESTRUCCION
void vaciar_arbol(tArbol *pa);
///INSERCION
int insertar_rec_arbol(tArbol *p, const void* pd, size_t tam, int(*cmp)(const void*, const void*));
///BUSQUEDA
tNodoA** buscar_nodo_arbol(tArbol *pa, void* pd,int(*cmp)(const void*, const void*));
int buscar_elem_arbol(tArbol *pa, void * pd, unsigned tam,int(*cmp)(const void*,const void*));
///Recorrido en orden
void recorrer_en_orden_rec(tArbol *pa,size_t nivel,void* params, void(*accion)(void*,size_t,size_t,void*));
void recorrer_en_orden(tArbol *pa,void* params, void(*accion)(void*,size_t,size_t,void*)); //función "envoltorio", llama a la recursiva de arriba

#endif // ARBOL_H_INCLUDED
