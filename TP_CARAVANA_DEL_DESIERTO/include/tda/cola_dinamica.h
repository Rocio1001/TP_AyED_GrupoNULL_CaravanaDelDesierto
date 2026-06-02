#ifndef COLA_DINAMICA_H_INCLUDED
#define COLA_DINAMICA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TODO_OK 1
#define COLA_LLENA -1
#define COLA_VACIA -2
#define MIN(X,Y)((X)<(Y)?(X):(Y))

typedef struct sNodo{
    void* info;
    size_t tamInfo;
    struct sNodo* sig;
}tNodo;

typedef struct{
    tNodo* pri;
    tNodo* ult;
}tCola;

void crear_cola(tCola* pc);
void vaciar_cola(tCola* pc);
int cola_llena(const tCola* pc, size_t tam);
int cola_vacia(const tCola* pc);
int insertar_en_cola(tCola* pc, const void* pd, size_t tam);
int sacar_de_cola(tCola* pc, void* pd, size_t tam);
int ver_frente_cola(const tCola* pc, void* pd, size_t tam);




#endif // COLA_DINAMICA_H_INCLUDED
