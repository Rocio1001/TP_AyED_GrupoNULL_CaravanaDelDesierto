#include "../../include/tda/lista_simple.h"

///******************************************************************************************
///  PRIMITIVAS  MINIMAS Y NECESARIAS PARA HACER FUNCIONAR EL RANKING
///******************************************************************************************



///operaciones b�sicas
void crear_lista(tLista* pl){
    *pl = NULL;
}
void vaciar_lista (tLista* pl){
    tNodo* aux;
    while(*pl){
        aux = *pl;
        *pl = aux->sig;
        free(aux->info);
        free(aux);
    }
}

///insertar ordenado
int insertar_ordenado_lista(tLista* pl, const void* pd, size_t tam,int aceptDup, int(*cmp)(const void*, const void*),void(*accion)(void**,size_t*,const void*,size_t)){
    tNodo* nue;
    int resCmp;

    while(*pl && (resCmp = cmp(pd,(*pl)->info))>0 ){
        pl = &(*pl)->sig;
    }
    if(resCmp == 0){ //SI SON IGUALES LOS DATOS A INGRESAR
        if(!aceptDup){ //si no acepta duplicados, entro
            if(accion){ //si hay accion a aplicar, aplico accion
                accion(&(*pl)->info, &(*pl)->tamInfo, pd, tam); //aplico accion
            }
            return DUPLICADO;
        }
    }
    nue = (tNodo*)malloc(sizeof(tNodo));
    if(!nue)
        return LISTA_LLENA;
    nue->info = malloc(tam);
    if(!nue->info){
        free(nue);
        return LISTA_LLENA;
    }
    nue->tamInfo = tam;

    memcpy(nue->info,pd,tam);
    nue->sig = *pl;
    *pl = nue;
    return TODO_OK;
}

///ordenar
//primitiva de ordenamiento que me enseñaron el cuatri pasado, evaluar si esta es la mejor forma de ordenar o si queremos elegir otro algoritmo de ordenamiento
void ordenar_lista(tLista *pl, int (*cmp)(const void*, const void*)){
    tLista *pri, *listaAux;
    tNodo *nodoAux;

    pri = pl; // guardamos la dirección del puntero al inicio de la lista

    if(!*pl)
        return; // si la lista esta vacia, no hay nada que ordenar

    while((*pl)->sig){
        // Si el actual es MAYOR al siguiente, hay que reubicar al siguiente
        if(cmp((*pl)->info, (*pl)->sig->info) > 0){

            // extraemos el nodo a reubicar
            nodoAux = (*pl)->sig;
            (*pl)->sig = nodoAux->sig; // lo desenganchamos

            // "volvemos" al inicio para buscar su lugar
            listaAux = pri;

            // avanzamos mientras el nodo evaluado sea MENOR al nodoAux
            while(cmp((*listaAux)->info, nodoAux->info) <= 0) {
                listaAux = &(*listaAux)->sig;
            }

            // el nodo a insertar debe apuntar a lo que actualmente apunta listaAux
            nodoAux->sig = *listaAux;

            // enganchamos el nodo a la lista
            *listaAux = nodoAux;

        } else {
            // si están ordenados, simplemente avanzamos
            pl = &(*pl)->sig;
        }
    }
}

///recorrido
void recorrer_lista(tLista * pl, void (*accion)(const void*,size_t, void*), void* param){

    while(*pl){
        accion((*pl)->info, (*pl)->tamInfo, param);
        pl = &(*pl)->sig;
    }

}
