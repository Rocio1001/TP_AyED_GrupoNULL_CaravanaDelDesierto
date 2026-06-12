#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "include/juego/partida.h"
#include "include/juego/menu.h"
#include "include/sistema_de_puntos/ranking.h"
#include "include/sesion_e_historico/historico.h"

#include "include/sesion_e_historico/sesion_y_alta.h"

#define RUTA_HISTORICO_BIN "historico_partidas.bin"
#define RUTA_MAESTRO_JUGADORES_BIN "maestro_jugadores.bin"
#define FIN_JUEGO 0

int main(){
    int opcion, resultado;
    char buffer[16];
    tPartida partida;
    tHistorico historico;

    srand((unsigned int)time(NULL));


/// -------------------- INICIO SESIÓN -------------------------
    resultado = iniciar_sesion(&partida.jugador, RUTA_MAESTRO_JUGADORES_BIN);
    if(resultado == SESION_ALTA)
    {
        printf("  Usuario cargado con ID: %d\n", partida.jugador.id);
    }
    else{
        ///fin programa;
        printf("  Cerrando el juego...\n");
        return FIN_JUEGO;
    }

    system("pause");
/// ------------- JUEGO CARAVANA DEL DESIERTO --------------------
    do {
        system("cls");

        mostrar_opciones();
        //solo para ver si estaba seleccionando bien
        ///borrar posteriormente
        printf("\nEsto es para probar que este todo oki\n");
        printf("Jugador ID: %d\n",partida.jugador.id);
        printf("Jugador Nombre: %s\n",partida.jugador.nombre);
        printf("\n");

        fgets(buffer, sizeof(buffer), stdin);
        opcion = atoi(buffer);

        switch(opcion){
            case 1:{

                system("cls");
                printf("  Preparando configuracion...\n\n");

                if(inicializar_partida(&partida, RUTA_CONFIG) == TODO_OK){
                    ejecutar_partida(&partida);

                    /* Guardar resultado en el historico binario */
                    historico.idJugador = partida.jugador.id;
                    strncpy(historico.nombreJugador, partida.jugador.nombre,
                            sizeof(historico.nombreJugador) - 1);
                    historico.nombreJugador[sizeof(historico.nombreJugador) - 1] = '\0';
                    historico.puntos      = partida.jugador.puntos;
                    historico.movimientos = partida.numero_turno;
                    escribir_historico(&historico, RUTA_HISTORICO_BIN);
                } else {
                    printf("\n  Error al inicializar la partida.\n");
                }

                finalizar_partida(&partida);

                printf("\n  Presione Enter para continuar...");
                while(getchar() != '\n');
                break;
            }
            case 2:
                /// --- VER RANKING ---
                system("cls");
                printf("  +-----------------------------------------+\n");
                printf("  |              VER RANKING                |\n");
                printf("  +-----------------------------------------+\n\n");
                crear_y_mostrar_ranking(RUTA_HISTORICO_BIN);
                printf("\n  Presione Enter para continuar...");
                while(getchar() != '\n');
                break;

            case 3:
                /// --- SALIR ---
                system("cls");
                printf("  Hasta la proxima!\n\n");
                break;

            default:
                printf("\n  Opcion invalida. Intente nuevamente.\n");
                printf("  Presione Enter para continuar...");
                while(getchar() != '\n');
                break;
        }

    } while(opcion != 3);

    ///esto es para probar que se haya hecho bien
    mostrar_historico(RUTA_HISTORICO_BIN);

    return FIN_JUEGO;
}
