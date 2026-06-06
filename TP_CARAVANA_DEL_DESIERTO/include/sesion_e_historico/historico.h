#ifndef HISTORICO_H_INCLUDED
#define HISTORICO_H_INCLUDED


typedef struct{
    int idPartida;
    int idJugador;
    int nombreJugador[30];
    int puntos;
    int movimientos;
}tHistorico;

//puede ser un void
void escribir_historico(tHistorico *historico, const char *pathHistorico);

#endif // HISTORICO_H_INCLUDED
