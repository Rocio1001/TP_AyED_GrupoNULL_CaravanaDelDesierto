#ifndef BANDIDO_H_INCLUDED
#define BANDIDO_H_INCLUDED

// FORWARD DECLARATION: Le avisamos al compilador que este struct existe.
struct sNodoD;


typedef struct{
    int id;
    struct sNodoD* posicion;
}tBandido;


#endif // BANDIDO_H_INCLUDED
