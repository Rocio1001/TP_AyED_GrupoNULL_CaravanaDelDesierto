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
    char nombre[30];
}tClave;
typedef struct{
    tClave clave;
    size_t valor; ///posicion
}tIndice;

typedef struct{
    int id;
    char nombre[30];
}tJugadorMaestro;

int cmp_jugadores(const void *a, const void *b); ///cmp

int posicion_final_en_archivo(const char *nombArch);///jugador nuevo

void buscar_en_maestro(tJugadorMaestro *jugador, tIndice *indice, const char *nombArch);

void cargar_arbol_indice_maestro_jugadores(tArbol *arbol, FILE **pf);///cargar jugadoresMaestro

int guardarMaestroJugadores (tArbol *arbol, const char* nombArch);///guardar jugadoresMaestro

int registar_jugador(tArbol *arbol, tJugadorMaestro *jugador, const char *nombArch);///jugador nuevo

int dar_alta_jugador_arbol(tArbol *arbol, tJugadorMaestro *jugador, const char *nombArch);///jugador nuevo

int dar_alta_jugador_maestro(const char* pathMaestro, tJugadorMaestro *jugador);

int cargar_jugador_existente(tArbol *arbol, tJugador *jugador, const char *nombArch);///jugador existente

int alta_jugador (tArbol *arbol, tJugadorMaestro *jugador, const char *nombArch); ///envoltorio

int procesar_alta_y_mapeo(tArbol *arbol, tJugador *jugador, tJugadorMaestro *jugadorSesion, const char *nombArch); ///envoltorio

int iniciar_sesion (tJugador *jugador, const char *nombArchMaestro);///sesion

#endif // SESION_Y_ALTA_H_INCLUDED
