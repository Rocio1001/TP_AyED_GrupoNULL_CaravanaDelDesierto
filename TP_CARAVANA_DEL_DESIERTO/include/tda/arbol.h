#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define TODO_OK 1
#define DUPLICADO -2
#define SIN_MEM -1
#define ERROR_ARCH -4
#define MIN(X,Y) ((X)<(Y)?(X):(Y))
#define MAX(X,Y) ((X)>(Y)?(X):(Y))

typedef struct sNodoA{
    void* info;
    size_t tamInfo;
    struct sNodoA *izq;
    struct sNodoA *der;
}tNodoA;

typedef tNodoA *tArbol;


///******************************************************************************************
///     Evaluar qué funciones vamos a usar, algunas pueden estar de más y otras pueden faltar.

///considero que vamos a necesitar balancear el arbol si y solo si, el archivo maestro de jugadores viene ordenado por valor clave a insertar en el arbol. evaluar.

///******************************************************************************************



///creacion
void crearArbol(tArbol *pa);
///destruccion
void vaciarArbol(tArbol *pa);
///inserción recursiva
int insertarRecArbolBinBusq(tArbol *pa, const void* pd, size_t tam, int(*cmp)(const void*, const void*));
///busqueda nodo clave
tNodoA** buscarNodoArbolBinBusq(tArbol *pa, void* pd,int(*cmp)(const void*, const void*));
///busqueda elemento clave
int buscarElemArbolBinBusq(tArbol *pa, void * pd, unsigned tam,int(*cmp)(const void*,const void*));
///RECORRIDO
void recorrerEnOrdenRecArbolBinBusq(tArbol *pa,size_t nivel,void* params, void(*accion)(void*,size_t,size_t,void*));
void recorrerEnOrdenArbolBinBusq(tArbol *pa,void* params, void(*accion)(void*,size_t,size_t,void*)); //función "envoltorio", llama a la recursiva de arriba
///mayor y menor
tNodoA** menorNodoArbolBinBusq(const tArbol *pa);
tNodoA** mayorNodoArbolBinBusq(const tArbol *pa);
//altura, nivel, cantidad total de nodos del arbol
unsigned alturaArbolBin(const tArbol *pa);
///eliminar
//no sé si vamos a necesitar eliminar, dado que NO damos de bajas usuarios.
int eliminarRaizArbolBinBusq(tArbol *pa);
int eliminarElemArbolBinBusq(tArbol *pa, void *pd, size_t tam,int(*cmp)(const void*,const void*));



#endif // ARBOL_H_INCLUDED
