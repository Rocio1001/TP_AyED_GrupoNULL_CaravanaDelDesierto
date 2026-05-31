#include "../../include/tda/arbol.h"
///******************************************************************************************
///     Evaluar qué funciones vamos a usar, algunas pueden estar de más y otras pueden faltar.
///******************************************************************************************

///creacion y destruccion
void crearArbol(tArbol *pa){
    *pa = NULL;
}

void vaciarArbol(tArbol *pa){
    ///recorro en pos orden, condici�n de corte -> fin de arbol *pa == NULL

    if(!*pa)
        return;

    ///izq
    vaciarArbol(&(*pa)->izq);
    ///der
    vaciarArbol(&(*pa)->der);
    ///raiz
    free((*pa)->info); //libero info
    free(*pa); //libero el nodo completo
    *pa= NULL;
}

///inserción recursiva
int insertarRecArbolBinBusq(tArbol *pa, const void* pd, size_t tam, int(*cmp)(const void*, const void*)){
    tNodoA *nue;
    int resCmp;

    if(!*pa){ //cuando no hay arbol, inserto
            nue = (tNodoA*)malloc(sizeof(tNodoA));
            if(!nue)
                return SIN_MEM;
            nue->info = malloc(tam);
            if(!nue->info){
                free(nue);
                return SIN_MEM;
            }

            memcpy(nue->info,pd, tam);
            nue->tamInfo = tam;
            nue->izq = NULL;
            nue->der = NULL;

            *pa = nue;
            return TODO_OK;
        }
        //cuando hay arbol, comparo
        resCmp = cmp(pd,(*pa)->info);
        if(resCmp < 0)
            return insertarRecArbolBinBusq(&(*pa)->izq,pd,tam,cmp);
        else if(resCmp>0)
            return insertarRecArbolBinBusq(&(*pa)->der,pd,tam,cmp);
        return DUPLICADO;
}

///busqueda de nodo clave
tNodoA** buscarNodoArbolBinBusq(tArbol *pa, void* pd,int(*cmp)(const void*, const void*)){
    int resCmp;

    while(*pa && (resCmp = cmp(pd,(*pa)->info)) ){
        if(resCmp<0)
            pa  = &(*pa)->izq;
        else if(resCmp > 0)
            pa  = &(*pa)->der;
    }
    if(!*pa)
        return NULL;

    return (tNodoA**)pa;
}
///busqueda de elemento clave
int buscarElemArbolBinBusq(tArbol *pa, void * pd, unsigned tam,int(*cmp)(const void*,const void*)){

    if(!(pa= buscarNodoArbolBinBusq(pa,pd,cmp))) //primero busco al nodo, deposito la direcci�n de memoria en "pa"
        return 0; /// NO_EXISTE

    memcpy(pd, (*pa)->info,MIN(tam,(*pa)->tamInfo));

    return TODO_OK;
}

///recorrido en orden
//considero que vamos a necesitarlo si vamos a mantener y "actualizar" un archivo jugadores.idx, falta más análisis
void recorrerEnOrdenRecArbolBinBusq(tArbol *pa,size_t nivel,void* params, void(*accion)(void*,size_t,size_t,void*)){
    if(!*pa)
        return;
    /*I*/recorrerEnOrdenRecArbolBinBusq(&(*pa)->izq,nivel+1,params,accion);
    /*R*/accion((*pa)->info,(*pa)->tamInfo,nivel,params);
    /*D*/recorrerEnOrdenRecArbolBinBusq(&(*pa)->der,nivel+1,params,accion);
}
void recorrerEnOrdenArbolBinBusq(tArbol *pa,void* params, void(*accion)(void*,size_t,size_t,void*)){ ///funci�n "envoltorio", llama a la recursiva de arriba
    recorrerEnOrdenRecArbolBinBusq(pa,0,params,accion);
}

//funciones necesarias para al eliminación de un nodo
unsigned alturaArbolBin(const tArbol *pa){
    unsigned hi, hd; //hi -> altura izq, hd-> altura derecha
    if(!*pa) //cuando llega a un nodo hoja,retorno 0
        return 0;
    hi = alturaArbolBin(&(*pa)->izq); //calculo la altura del subarbol izq
    hd = alturaArbolBin(&(*pa)->der); //calculo la altura del subarbol der
    return (hi>hd ? hi : hd) + 1; //retorno la altura mayor + 1 (por la altura del nodo raiz)
}
tNodoA** mayorNodoArbolBinBusq(const tArbol *pa){
    if(!*pa)
        return NULL;
    while((*pa)->der)
        pa = &(*pa)->der;
    return (tNodoA**)pa;
}
tNodoA** menorNodoArbolBinBusq(const tArbol *pa){
    if(!*pa)
        return NULL;
    while((*pa)->izq)
        pa = &(*pa)->izq;
    return (tNodoA**)pa;
}

//evaluar si vamos a eliminar algun nodo
int eliminarRaizArbolBinBusq(tArbol *pa){
    tNodoA **remp, *elim;

    if(!*pa)
        return 0;

    free((*pa)->info);
    if(!(*pa)->izq && !(*pa)->der)
    {
        free(*pa);
        *pa = NULL;
        return 1;///OK
    }

    remp = alturaArbolBin(&(*pa)->izq) > alturaArbolBin(&(*pa)->der)?
            mayorNodoArbolBinBusq(&(*pa)->izq):
            menorNodoArbolBinBusq(&(*pa)->der);

    elim = *remp;
    (*pa)->info = elim->info;
    (*pa)->tamInfo = elim->tamInfo;

    *remp = elim->izq ? elim->izq : elim->der;

    free(elim);

    return 1;///OK
}
int eliminarElemArbolBinBusq(tArbol *pa, void *pd, size_t tam,int(*cmp)(const void*,const void*)){
    if(!(pa=buscarNodoArbolBinBusq(pa,pd,cmp)))
        return 0; /// NO_EXISTE

    memcpy(pd,(*pa)->info,MIN(tam,(*pa)->tamInfo));
    return eliminarRaizArbolBinBusq(pa);
}


