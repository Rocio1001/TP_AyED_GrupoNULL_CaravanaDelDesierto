#include "../../include/tda/arbol.h"


///creacion
void crear_arbol(tArbol *pa){
    *pa = NULL;
}
///==============================================///


///destruccion
void vaciar_arbol(tArbol *pa){
    ///recorro en pos orden, condición de corte -> fin de arbol *pa == NULL

    if(!*pa)
        return;

    ///izq
    vaciar_arbol(&(*pa)->izq);
    ///der
    vaciar_arbol(&(*pa)->der);
    ///raiz
    free((*pa)->info); //libero info
    free(*pa); //libero el nodo completo
    *pa= NULL;
}
///==============================================///


///Insercion
int insertar_rec_arbol(tArbol *pa, const void* pd, size_t tam, int(*cmp)(const void*, const void*)){
    tNodoA *nue;
    int resCmp;

    if(*pa){
        if((resCmp=cmp(pd,(*pa)->info))<0)
            return insertar_rec_arbol(&(*pa)->izq,pd,tam,cmp);
        else if(resCmp>0)
            return insertar_rec_arbol(&(*pa)->der,pd,tam,cmp);
        else
            return CLA_DUP;
    }

    nue=malloc(sizeof(tNodoA));
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
///==============================================///


///Busqueda
tNodoA** buscar_nodo_arbol(tArbol *pa, void* pd,int(*cmp)(const void*, const void*)){
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
int buscar_elem_arbol(tArbol *pa, void * pd, unsigned tam,int(*cmp)(const void*,const void*)){

    if(!(pa= buscar_nodo_arbol(pa,pd,cmp))) //primero busco al nodo, deposito la dirección de memoria en "pa"
        return 0; /// NO_EXISTE

    memcpy(pd, (*pa)->info,MIN(tam,(*pa)->tamInfo));

    return TODO_OK;
}
///=============================================///

///recorrido en orden
void recorrer_en_orden_rec(tArbol *pa,size_t nivel,void* params, void(*accion)(void*,size_t,size_t,void*)){
    if(!*pa)
        return;
    /*I*/recorrer_en_orden_rec(&(*pa)->izq,nivel+1,params,accion);
    /*R*/accion((*pa)->info,(*pa)->tamInfo,nivel,params);
    /*D*/recorrer_en_orden_rec(&(*pa)->der,nivel+1,params,accion);
}
void recorrer_en_orden(tArbol *pa,void* params, void(*accion)(void*,size_t,size_t,void*)){ ///función "envoltorio", llama a la recursiva de arriba
    recorrer_en_orden_rec(pa,0,params,accion);
}
///==============================================///
