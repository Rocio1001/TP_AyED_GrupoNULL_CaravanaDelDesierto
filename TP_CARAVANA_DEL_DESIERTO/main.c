#include <stdio.h>
#include <stdlib.h>

#define RUTA_CONFIG "config.txt" //evaluar si estas macros van aca o en "tablero.h"
#define RUTA_CARAVANA "caravana.txt" //evaluar si estas macros van aca o en "tablero.h"

#include "include/juego/partida.h"
#include "include/sistema/ranking.h"
//#include menu.h
//#include sesion.h
int main(){
    ///COSAS A PENSAR Y HACER
    /*

        - RANKING DESARROLLADO - HECHO. hay criterios a analizar y evaluar, los cuales estan comentados en el mismo ranking.c

        - evaluar cómo manejar el error porque se incluye cola y lista, los cuales definen el tNodo (actualmente, la inclusión de cola.h a movimiento.h esta comentada por esta razon)

        - evaluar que primitivas de arbol van a ser utiles para nuestro "inicio de sesion y alta" de usuarios

        - evaluar nombres descriptivo de la carpeta "sistema" para guardar el modulo del ranking y del "alta" de usuarios

        - corregir el desarrollo de tablero.c, faltan algunas validaciones de reglas del negocio y casos borders a evaluar. (EN PROCESO)

    */
    crear_y_mostrar_ranking("historico_partidas.txt"); //ruta del archivo-> debemos definirla en una MACRO, FALTA hacerlo. Definir cuál es el mejor lugar para hacerlo.

    return 0;
}
