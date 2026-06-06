#ifndef NODO_H_INCLUDED
#define NODO_H_INCLUDED

#include <stdlib.h>

/*
 * Nodo generico compartido por lista_simple y cola_dinamica.
 * Centralizado aqui para evitar redefinicion de tNodo cuando ambos
 * headers se incluyen en la misma unidad de compilacion.
 */
typedef struct sNodo{
    void*         info;
    size_t        tamInfo;
    struct sNodo* sig;
}tNodo;

#endif // NODO_H_INCLUDED
