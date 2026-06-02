#include <stdio.h>
#include <stdlib.h>

#define RUTA_CONFIG "config.txt"
#define RUTA_CARAVANA "caravana.txt"
#include "include/juego/partida.h"
#define RUTA

int main(){
    //tPartida miPartida;
    ///PRUEBA DE GENERAR UN TABLERO.
    srand(time(NULL));
    tTablero tablero;
    tBandido *bandidos;
    tJugador jugador;
    crear_tablero(&tablero);
    if(construir_tablero(&tablero,&bandidos,&jugador,RUTA_CONFIG) != TABLERO_GENERADO){
        destruir_tablero(&tablero);
        return 0;
    }
    actualizar_caravana_txt(&tablero,RUTA_CARAVANA);
    mostrar_caravana_txt(RUTA_CARAVANA);
    system("pause");
    ///========================================

    /*
    //Probar dado y direccion
    int dado, direccion_mov;

    printf("Presione un tecla para tirar el dado...\n");
    dado = tirarDado();
    printf("Seleccione una direccion Avanzar (1) o Retroceder (-1)\n");
    scanf("%d", &direccion_mov);
    printf("MOVIMIENTO >> %d\n", direccion_mov * dado);
    system("pause");
    */

    /// guardo el movimiento de jugador
    int direccion_mov;
    tMovimiento movimiento;


    ///para testear nomas
    printf("Cantidad de pasos: ");
    scanf("%d", &movimiento.pasos);


    movimiento.tipo = TIPO_JUGADOR;
    movimiento.entidad = &jugador;
    //movimiento.pasos = tirarDado();
    printf("Resultado del dado: %d\n", movimiento.pasos);
    printf("Seleccione una direccion Avanzar (1) o Retroceder (-1)\n");
    scanf("%d", &direccion_mov);
    movimiento.direccion = direccion_mov;

    /// hay que encolar este movimiento
    tCola colaMovimientos;

    crear_cola(&colaMovimientos);
    insertar_en_cola(&colaMovimientos, &movimiento, sizeof(tMovimiento));

    /// hay que volcar los movimientos encolados en el tablero y actualizarlo
    actualizar_tablero( &tablero, &colaMovimientos);
    printf("Actualizando mapa....\n");
    system("pause");
    mostrar_caravana_txt(RUTA_CARAVANA);

    ///========================================
    destruir_tablero(&tablero);
    return 0;
}
