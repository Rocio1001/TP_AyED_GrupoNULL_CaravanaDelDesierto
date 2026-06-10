/* *******************************
Nota / Aclarcion en el .c
******************************* */

#ifndef SESION_Y_ALTA_H_INCLUDED
#define SESION_Y_ALTA_H_INCLUDED

#include "../tda/arbol.h"
#include "../entidad/jugador.h"
#include "../juego/menu.h"

#define JUGADOR_NO_ENCONTRADO 0 //esta tiene que quedar como 0, los otros valores creo que se pueden cambiar y no pasria nada (creo)
#define JUGADOR_CREADO 1
#define JUGADOR_ENCONTRADO 2

#define SESION_ERROR -2
#define SESION_ALTA 3

//Registros de jugadoresMaestro.bin
typedef struct{
    int id;
    char nombre[30];
}tJugadorMaestro;

int compararJugadores(const void *a, const void *b); ///cmp

int generarId(FILE *pf);///jugador nuevo

void cargarMaestroJugadores(tArbol *arbol, FILE *pf);///cargar jugadoresMaestro

int guardarMaestroJugadores (tArbol *arbol, const char* nombArch);///guardar jugadoresMaestro

int registrarJugador(tArbol *arbol, tJugador *jugador, FILE *pf);///jugador nuevo

int darJugadorDeAlta(tArbol *arbol, tJugador *jugador, FILE *pf);///jugador nuevo

int cargarJugadorExistente(tArbol *arbol, tJugador *jugador);///jugador existente

int altaYGuardar (tArbol *arbol, tJugador *jugador, FILE **pMaestro, const char *nombArch);

int iniciar_sesion (tJugador *jugador, const char *nombArchMaestro);///sesion

#endif // SESION_Y_ALTA_H_INCLUDED
