#ifndef LISTA_SIMPLE_H_INCLUDED
#define LISTA_SIMPLE_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TODO_OK 1
#define LISTA_LLENA -1
#define LISTA_VACIA -2
#define DUPLICADO -3
#define NO_ENCONTRADO -4
#define POS_INVALIDA -5
#define MIN(X,Y)((X)<(Y)?(X):(Y))

typedef struct sNodo{
    void* info;
    size_t tamInfo;
    struct sNodo* sig;
}tNodo;

typedef tNodo* tLista;

///operaciones básicas
void crear_lista(tLista* pl);
void vaciar_lista (tLista* pl);
int lista_llena(const tLista* pl, size_t tam);
int lista_vacia(const tLista* pl);
///operaciones al principio
int insertar_en_lista(tLista* pl, const void* pd, size_t tam);
int ver_primero_lista(const tLista* pl, void* pd, size_t tam);
int sacar_de_lista(tLista* pl, void* pd, size_t tam);
///operaciones al final
int insertar_final_lista(tLista* pl, const void* pd, size_t tam);
int ver_ultimo_lista(const tLista* pl, void* pd, size_t tam);
int sacar_ultimo_lista(tLista* pl, void* pd, size_t tam);
///operaciones en posición determinada
int insertar_en_pos_lista(tLista* pl, const void* pd, size_t tam,int pos);
int ver_en_pos_lista(const tLista* pl, void* pd, size_t tam,int pos);
int sacar_de_pos_lista(tLista* pl, void* pd, size_t tam,int pos);

void recorrer_lista(tLista * pl, void (*accion)(const void*,size_t, void*), void* param);
///ordenado
int insertar_ordenado_lista(tLista* pl, const void* pd, size_t tam, int(*cmp)(const void*, const void*),void(*accion)(void**,size_t*,const void*,size_t));

#endif // LISTA_SIMPLE_H_INCLUDED
