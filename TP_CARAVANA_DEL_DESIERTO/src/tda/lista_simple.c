#include "../../include/tda/lista_simple.h"



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
int lista_llena(const tLista* pl, size_t tam){
    tNodo* aux = malloc(sizeof(tNodo));
    void* info = malloc(tam);
    free(aux);
    free(info);
    return (!info || !aux ) ? LISTA_LLENA : TODO_OK;
}
int lista_vacia(const tLista* pl){
    return !*pl ? LISTA_VACIA : TODO_OK;
}
///operaciones al principio
int insertar_en_lista(tLista* pl, const void* pd, size_t tam){
    tNodo* nue;
    nue = malloc(sizeof(tNodo));
    if(!nue)
        return LISTA_LLENA;
    nue->info = malloc(tam);
    if(!nue->info){
        free(nue);
        return  LISTA_LLENA;
    }
    memcpy(nue->info,pd,tam);
    nue->tamInfo = tam;
    nue->sig = *pl;
    *pl = nue;
    return TODO_OK;
}
int ver_primero_lista(const tLista* pl, void* pd, size_t tam){
    tNodo* aux;
    if(!*pl)
        return LISTA_VACIA;
    aux = *pl;
    memcpy(pd,aux->info,MIN(tam,aux->tamInfo));
    return TODO_OK;
}
int sacar_de_lista(tLista* pl, void* pd, size_t tam){
    tNodo* aux;
    if(!*pl)
        return LISTA_VACIA;
    aux = *pl;
    *pl = aux->sig;
    memcpy(pd,aux->info,MIN(tam,aux->tamInfo));
    free(aux->info);
    free(aux);
    return TODO_OK;
}
///operaciones al final
int insertar_final_lista(tLista* pl, const void* pd, size_t tam){
    tNodo* nue;
    while(*pl)
        pl = &(*pl)->sig;
    nue = malloc(sizeof(tNodo));
    if(!nue)
        return LISTA_LLENA;
    nue->info = malloc(tam);
    if(!nue->info){
        free(nue);
        return  LISTA_LLENA;
    }
    memcpy(nue->info,pd,tam);
    nue->tamInfo = tam;
    nue->sig = NULL; //si hacemos nue->sig = *pl es lo mismo, dado que estamos al final con pl :D
    *pl = nue;
    return TODO_OK;
}
int ver_ultimo_lista(const tLista* pl, void* pd, size_t tam){
    tNodo* aux;
    if(!*pl)
        return LISTA_VACIA;
    while((*pl)->sig)
        pl = &(*pl)->sig;
    aux = *pl;
    memcpy(pd,aux->info,MIN(tam,aux->tamInfo));
    return TODO_OK;
}
int sacar_ultimo_lista(tLista* pl, void* pd, size_t tam){
    tNodo* aux;
    if(!*pl)
        return LISTA_VACIA;
    while((*pl)->sig)
        pl = &(*pl)->sig;
    aux = *pl;
    *pl = NULL;
    memcpy(pd,aux->info,MIN(tam,aux->tamInfo));
    free(aux->info);
    free(aux);
    return TODO_OK;
}
