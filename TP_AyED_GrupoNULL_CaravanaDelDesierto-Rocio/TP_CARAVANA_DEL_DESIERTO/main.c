#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "include/juego/partida.h"
#include "include/juego/menu.h"
#include "include/sistema_de_puntos/ranking.h"

int main(){
    int opcion;
    char buffer[16];

    srand((unsigned int)time(NULL));

    do {
        system("cls");
        mostrar_opciones();

        fgets(buffer, sizeof(buffer), stdin);
        opcion = atoi(buffer);

        switch(opcion){
            case 1:{
                /// --- JUGAR PARTIDA ---
                tPartida partida;

                system("cls");
                printf("  Preparando configuracion...\n\n");

                if(inicializar_partida(&partida, RUTA_CONFIG) == TODO_OK){
                    ejecutar_partida(&partida);
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
                crear_y_mostrar_ranking("historico_partidas.txt");
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

    return 0;
}
