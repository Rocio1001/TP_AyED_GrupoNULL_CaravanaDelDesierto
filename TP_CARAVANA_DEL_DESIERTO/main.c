#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "include/juego/partida.h"
#include "include/juego/menu.h"
#include "include/sistema_de_puntos/ranking.h"
#include "include/sesion_e_historico/historico.h"

#define RUTA_HISTORICO_BIN "historico_partidas.bin"

///función para chequear que todo este oki
void mostrar_historico(const char* file_name) {

    tHistorico historico;
    FILE* pArchivo = fopen(file_name, "rb");

    if(pArchivo == NULL) {
        printf("Error al abrir el archivo\n");
    }

    printf("  +-----------------------------------------+\n");
    printf("  |             HISTORICO PARTIDAS          |\n");
    printf("  +-----------------------------------------+\n");


    fread(&historico, sizeof(tHistorico), 1, pArchivo);
    while(!feof(pArchivo)) {

        printf("ID Partida: %-4d | ID Jugador: %-4d | Nombre Jugador: %-30s | Puntos: %-4d | Movimientos: %-4d \n",
            historico.idPartida,
            historico.idJugador,
            historico.nombreJugador,
            historico.puntos,
            historico.movimientos);

        fread(&historico, sizeof(tHistorico), 1, pArchivo);
    }

    fclose(pArchivo);
}

int main(){
    int opcion;
    char buffer[16];
    tPartida partida;
    tHistorico historico;

    srand((unsigned int)time(NULL));

    do {
        system("cls");
        mostrar_opciones();

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

    return 0;
}
