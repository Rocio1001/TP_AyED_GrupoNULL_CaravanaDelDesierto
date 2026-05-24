#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED


// FORWARD DECLARATION: Le avisamos al compilador que este struct existe.
struct sNodoD;


typedef struct{
    int id;
    char nombre[30];
    char nombreInterno[40];
    int proteccion;
    int turnoValido;
    int puntos;
    struct sNodoD* posicion;
}tJugador;



#endif // JUGADOR_H_INCLUDED
