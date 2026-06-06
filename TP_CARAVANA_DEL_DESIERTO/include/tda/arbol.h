#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CLA_DUP -2
#define SIN_MEM -1
#define SIN_INICIALIZAR -3
#define ERROR_ARCH -4
#define TODO_OK 1
#define MIN(X,Y) ((X)<(Y)?(X):(Y))
#define MAX(X,Y) ((X)>(Y)?(X):(Y))

typedef struct sNodo{
    void* info;
    size_t tamInfo;
    struct sNodo *izq;
    struct sNodo *der;
}tNodoA;

typedef tNodoA *tArbol;

///CREACION
void crearArbol(tArbol *pa);
///DESTRUCCION
void vaciarArbol(tArbol *pa);
void podarArbol(tArbol *pa);
///INSERCION
int insertarArbolBinBusq(tArbol *pa, const void* pd, size_t tam, int(*cmp)(const void*, const void*));
int insertarRecArbolBinBusq(tArbol *p, const void* pd, size_t tam, int(*cmp)(const void*, const void*));
int insertarDirectoArbolBinBusq(tArbol *p, const void* pd, size_t tam, int(*cmp)(const void*, const void*));
///BUSQUEDA DE NODO
tNodoA** buscarRecNodoArbolBinBusq(tArbol *pa, void* pd,int(*cmp)(const void*, const void*));
tNodoA** buscarNodoArbolBinBusq(tArbol *pa, void* pd,int(*cmp)(const void*, const void*));
///BUSQUEDA ELEMENTO
int buscarElemArbolBinBusq(tArbol *pa, void * pd, unsigned tam,int(*cmp)(const void*,const void*));
///IMPRIMIR ARBOL CON FORMA
void imprimirArbolConForma(void* info, unsigned tam, unsigned nivel, void* params);
///RECORRIDOS
//pre orden, RID
void recorrerPreOrdenRecArbolBinBusq(tArbol *pa,size_t nivel,void* params, void(*accion)(void*,size_t,size_t,void*));
void recorrerPreOrdenArbolBinBusq(tArbol *pa,void* params, void(*accion)(void*,size_t,size_t,void*)); //función "envoltorio", llama a la recursiva de arriba
//en orden, IRD
void recorrerEnOrdenRecArbolBinBusq(tArbol *pa,size_t nivel,void* params, void(*accion)(void*,size_t,size_t,void*));
void recorrerEnOrdenArbolBinBusq(tArbol *pa,void* params, void(*accion)(void*,size_t,size_t,void*)); //función "envoltorio", llama a la recursiva de arriba
//pos orden, IDR
void recorrerPosOrdenRecArbolBinBusq(tArbol *pa,size_t nivel,void* params, void(*accion)(void*,size_t,size_t,void*));
void recorrerPosOrdenArbolBinBusq(tArbol *pa,void* params, void(*accion)(void*,size_t,size_t,void*)); //función "envoltorio", llama a la recursiva de arriba
//en orden inverso, DIR
void recorrerEnOrdenInversoRecArbolBinBusq(tArbol *pa,size_t nivel,void* params, void(*accion)(void*,size_t,size_t,void*));
void recorrerEnOrdenInversoArbolBinBusq(tArbol *pa,void* params, void(*accion)(void*,size_t,size_t,void*)); //función "envoltorio", llama a la recursiva de arriba
//recorridas simples, sin nivel
void recorrerPreOrdenSimpleArbolBinBusq(const tArbol *pa, void *params,void (*accion)(void *, size_t, void*));
void recorrerEnOrdenSimpleArbolBinBusq(const tArbol *pa, void *params,void (*accion)(void *, size_t, void*));
void recorrerPosOrdenSimpleArbolBinBusq(const tArbol *pa, void *params,void (*accion)(void *, size_t, void*));
///CONTEOS Y ESTADÍSTICAS
//mayor y menor
tNodoA** menorNodoArbolBinBusq(const tArbol *pa);
tNodoA** mayorNodoArbolBinBusq(const tArbol *pa);
int mayorElemArbolBinBusq(const tArbol *pa, void *pd, size_t tam);
int menorElemArbolBinBusq(const tArbol *pa, void *pd, size_t tam);
//altura, nivel, cantidad total de nodos del arbol
unsigned alturaArbolBin(const tArbol *pa);
unsigned cantNodosArbolBin(const tArbol *pa);
///ELIMINAR
int eliminarRaizArbolBinBusq(tArbol *pa);
int eliminarElemArbolBinBusq(tArbol *pa, void *pd, size_t tam,int(*cmp)(const void*,const void*));
///ARMAR ARBOL DE INDICES DESDE ARCHIVO ORDENADO
int cargarDesdeDatosOrdenadosRec(tArbol *pa, void *ds,
                                unsigned (*leer)(void **, void*, unsigned, void * params),
                                int li,int ls, void* params);
int cargarDesdeDatosOrdenadosArbolBinBusq(tArbol * p, void * ds, unsigned cantReg,
                                          unsigned (*leer)(void**,void*,unsigned,void*params),void* params);

///en caso de que el data set que posee los datos ordenados sea un archivo binario, podemos usar lo siguiente:
unsigned leerDesdeArchivoBin(void ** pd, void * pf,unsigned pos, void * params);
int cargarArchivoBinOrdenadoAbiertoArbolBinBusq(tArbol *pa, FILE * pf, size_t tamInfo);//si el archivo binario fue previamente abierto, usamos lo siguiente. Es una función envoltorio que prepara el llamado a la recursiva
int cargarArchivoBinOrdenadoArbolBinBusq(tArbol *p, const char * path,unsigned tamInfo);//si el archivo binario no fue abierto, usamos lo siguiente. Es una función envoltorio que prepara el llamado a la recursiva

#endif // ARBOL_H_INCLUDED
