#include "../../include/juego/movimiento.h"

///cambios ===========================================

int tirarDado()
{
    return (rand() % 6) + 1;
}


void aplicarMovimientos(tTablero* tablero, tCola* colaMovimientos)
{
    tNodoD* posFinal;
    tJugador* jugador;
    tMovimiento movimiento;
    tCasillero *casilleroFinal, *casilleroActual;

    //saco el primer movimiento de la cola (movimiento de jugador)
    sacar_de_cola(colaMovimientos, &movimiento, sizeof(tMovimiento));

    jugador = (tJugador*)movimiento.entidad; //apunto el puntero jugador al jugador (por medio de movimiento.entidad)

    //Hay que usar tablero partiendo de donde esta el jugador y aplicar el desplazamiento

    posFinal = moverEnTablero(tablero, jugador->posicion, movimiento.pasos, movimiento.direccion);

    casilleroFinal = (tCasillero*)posFinal->info;
    casilleroFinal->hayJugador = 1;

    casilleroActual = (tCasillero*)jugador->posicion->info;
    casilleroActual->hayJugador = 0;

    jugador->posicion = posFinal;
}

///creo que se puede usar tambien para bandidos despues
tNodoD* moverEnTablero (tTablero* tablero, tNodoD* origen, int pasos, int direccion)
{
    int i;
    //direccion esta para poder cambiar de sentido sin cambiar tMovimiento
    //Desde la posicion del jugador aplico el desplazamiento
    //si direccion es 1 avanza y si llega a tope va para atras los pasos que falten. Si dir -1 lo contrario
    for(i = 0; i < pasos; i++)
    {
        if(direccion == 1)
        {
            if( origen->sig == (tNodoD*)tablero->lista ) //si el que sigue es el primero, => se llego al final
            {
                direccion = -1;
                origen = origen->ant;
            }
            else
                origen = origen->sig;
        }
        else
        {
            if( origen->ant == (tNodoD*)tablero->lista->ant ) //si el que sigue es el ultimo, => se llego al inicio
            {
                direccion = 1;
                origen = origen->sig;
            }
            else
                origen = origen->ant;
        }
    }
    return origen;
}
