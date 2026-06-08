/* *******************************
Nota / Aclarcion en el .c
******************************* */

#ifndef SESION_Y_ALTA_H_INCLUDED
#define SESION_Y_ALTA_H_INCLUDED

#include "../tda/arbol.h"
#include "../entidad/jugador.h"
#include "../juego/menu.h"

#define JUGADOR_NO_ENCONTRADO 0
#define JUGADOR_CREADO 1
#define JUGADOR_ENCONTRADO 2

#define SESION_ERROR -1
#define SESION_ALTA 1

//Registros de jugadoresMaestro.bin
typedef struct{
    int id;
    char nombre[30];
}tJugadorMaestro;

int compararJugadores(const void *a, const void *b); ///cmp

int generarId(const char *nombArch);///jugador nuevo

void cargarMaestroJugadores(tArbol *arbol, const char *nombArch);///cargar jugadoresMaestro

int guardarMaestroJugadores (tArbol *arbol, const char* nombArch);///guardar jugadoresMaestro

int registrarJugador(tArbol *arbol, tJugador *jugador, const char *nombArch);///jugador nuevo

int darJugadorDeAlta(tArbol *arbol, tJugador *jugador, const char *nombArch);///jugador nuevo

int cargarJugadorExistente(tArbol *arbol, tJugador *jugador);///jugador existente

int iniciar_sesion (tJugador *jugador, const char *nombArchMaestro);///sesion

#endif // SESION_Y_ALTA_H_INCLUDED
