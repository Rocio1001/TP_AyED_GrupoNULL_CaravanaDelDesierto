#ifndef JUGADOR_H_INCLUDED
#define JUGADOR_H_INCLUDED


// FORWARD DECLARATION: Le avisamos al compilador que este struct existe.
struct sNodoD;


typedef struct{
    int id;
    char nombre[30];
    char nombreInterno[40];
    int proteccion; // 1 tiene, 0 no
    int turnoValido; //1 valido, 0 no
    int puntos; // 0 puntos inicialmente
    struct sNodoD* posicion;
}tJugador;



#endif // JUGADOR_H_INCLUDED
