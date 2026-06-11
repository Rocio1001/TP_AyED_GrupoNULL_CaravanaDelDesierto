#include "../../include/tda/cola_dinamica.h"
void crear_cola(tCola* pc){
    pc->pri = NULL;
    pc->ult = NULL;
}
void vaciar_cola(tCola* pc){
    tNodo* aux;
    while(pc->pri){
        aux = pc->pri;
        pc->pri = aux->sig;
        free(aux->info);
        free(aux);
    }
    pc->ult = NULL;
}
int cola_llena(const tCola* pc, size_t tam){
    tNodo* aux = malloc(sizeof(tNodo));
    void* info = malloc(tam);
    free(aux);
    free(info);
    return (!aux || !info) ? COLA_LLENA : TODO_OK;
}
int cola_vacia(const tCola* pc){
    return !pc->pri ? COLA_VACIA : TODO_OK;
}
int insertar_en_cola(tCola* pc, const void* pd, size_t tam){
    tNodo* nue;
    nue = malloc(sizeof(tNodo));
    if(!nue)
        return COLA_LLENA;
    nue->info = malloc(tam);
    if(!nue->info){
        free(nue);
        return COLA_LLENA;
    }
    memcpy(nue->info,pd,tam);
    nue->tamInfo = tam;
    nue->sig = NULL;

    if(!pc->pri){
        pc->pri = nue;
    }else{
        pc->ult->sig = nue;
    }
    pc->ult = nue;

    return TODO_OK;
}
int sacar_de_cola(tCola* pc, void* pd, size_t tam){
    tNodo* aux;
    if(!pc->pri)
        return COLA_VACIA;

    aux = pc->pri;
    pc->pri = aux->sig;
    memcpy(pd,aux->info,MIN(tam,aux->tamInfo));

    if(!pc->pri)
        pc->ult = NULL;

    free(aux->info);
    free(aux);
    return TODO_OK;
}
int ver_frente_cola(const tCola* pc, void* pd, size_t tam){
        tNodo* aux;
    if(!pc->pri)
        return COLA_VACIA;

    aux = pc->pri;
    memcpy(pd,aux->info,MIN(tam,aux->tamInfo));
    return TODO_OK;
}
