#include "../../include/tda/lista_circular_doble.h"

///operaciones básicas
void crear_lista_doble_cir(tListaDC *pl){
    *pl = NULL;
}

void vaciar_lista_doble_cir(tListaDC *pl){
    tNodoD *aux;
    while(*pl){
        aux = (*pl)->ant; //apunta al "ultimo nodo"

        if(aux == *pl){ //si solo queda un nodo
            *pl = NULL; //corte de control
        }

        else{
            (*pl)->ant = aux->ant; //conecto al "primero" con el nuevo "ultimo"

            aux->ant->sig = *pl; //conecto al nuevo ultimo con el primero
        }
        free(aux->info);
        free(aux);
    }

}

///inserción
int insertar_ult_lista_doble_cir(tListaDC *pl, const void *pd, size_t tam){
    tNodoD *nue, *ult;

    nue = malloc(sizeof(tNodoD));
    if(!nue)
        return LISTA_LLENA;
    nue->info = malloc(tam);
    if(!nue->info){
        free(nue);
        return LISTA_LLENA;
    }
    memcpy(nue->info, pd, tam);
    nue->tamInfo = tam;

    if(!*pl){//si esta vacia
        //entonces es el primer nodo
        nue->sig = nue;
        nue->ant = nue;
        *pl=nue;
    }else{ //ya hay nodos en la lista
        ult = (*pl)->ant;//ult apunta al "ultimo" de la lista

        //conecto el nodo a la lista
        nue->ant = ult; //el anterior al nodo, apunta al viejo ultimo
        nue->sig = *pl; //el siguiente al nodo, apunta al primero de la lista

        //conecto la lista al nodo
        (*pl)->ant = nue; //el primero nodo, apunta al nuevo ultimo
        ult->sig = nue; //el ultimo nodo, apunta al nuevo ultimo
    }
    return TODO_OK;
}
int insertar_pri_lista_doble_cir(tListaDC *pl, const void *pd, size_t tam){
    tNodoD *nue, *pri, *ult;
     nue = malloc(sizeof(tNodoD));
    if(!nue)
        return LISTA_LLENA;
    nue->info = malloc(tam);
    if(!nue->info){
        free(nue);
        return LISTA_LLENA;
    }
    memcpy(nue->info, pd, tam);
    nue->tamInfo = tam;

    if(!*pl){//si esta vacia
        //entonces es el primer nodo
        nue->sig = nue;
        nue->ant = nue;
    }else{
        pri = *pl; //pri apunta al viejo primer nodo
        ult = (*pl)->ant; //ult apunta al ultimo nodo

        //conecto el nodo a la lista
        nue->sig = pri; //el siguiente del nuevo apunta al viejo primero
        nue->ant = ult; //el nuevo primero, apunta al ultimo nodo

        //conecto la lista al nodo
        pri->ant = nue;// el viejo primero, apunta al nuevo primero
        ult->sig = nue; //el ultimo apunta al neuvo primero
    }
    *pl=nue; //actualizo el puntero de la lista.
    return TODO_OK;
}
///eliminar
int sacar_ult_lista_doble_cir(tListaDC *pl, void *pd, size_t tam){
    tNodoD *aux;

    if(!*pl)
        return LISTA_VACIA;
    aux = (*pl)->ant;

    //copio la info
    memcpy(pd,aux->info,MIN(tam,aux->tamInfo));

    if(aux == *pl){//si estoy sacando el unico nodo de la lista
        *pl = NULL;
    }else{
        //conecto al primero con el nuevo ultimo
        (*pl)->ant = aux->ant;
        //conecto al nuevo ultimo con el primero
        aux->ant->sig = *pl;
    }
    //libero
    free(aux->info);
    free(aux);
    return TODO_OK;
}
int sacar_pri_lista_doble_cir(tListaDC *pl, void *pd, size_t tam){
    tNodoD *aux;

    if(!*pl)
        return LISTA_VACIA;
    aux = *pl;

    //copio la info
    memcpy(pd,aux->info,MIN(tam,aux->tamInfo));

    if(aux->sig == *pl){//si estoy sacando el unico nodo de la lista
        *pl = NULL;
    }else{
        *pl = aux->sig; //el puntero a la lista, apunta al nuevo primero
        (*pl)->ant = aux->ant; //el anterior al nuevo primero, apunta al ultimo
        aux->ant->sig = *pl; //el ultimo, apunta al nuevo primero
    }
    //libero
    free(aux->info);
    free(aux);
    return TODO_OK;
}
//ver en lista
int ver_ult_lista_doble_cir(const tListaDC* pl, void *pd, size_t tam){
    tNodoD *aux;

    if(!*pl)
        return LISTA_VACIA;
    aux = (*pl)->ant;
    //copio la info
    memcpy(pd,aux->info,MIN(tam,aux->tamInfo));
    return TODO_OK;
}
int ver_pri_lista_doble_cir(const tListaDC* pl, void *pd, size_t tam){
    tNodoD *aux;

    if(!*pl)
        return LISTA_VACIA;
    aux = *pl;
    //copio la info
    memcpy(pd,aux->info,MIN(tam,aux->tamInfo));
    return TODO_OK;
}
