#include "../../include/sesion_e_historico/historico.h"

void escribir_historico(tHistorico* historico, const char* pathHistorico){
    FILE* pf;
    long cantReg;
    pf = fopen(pathHistorico, "a+b");
    if(!pf)
        return;
    fseek(pf, 0L, SEEK_END);
    cantReg = ftell(pf) / sizeof(tHistorico);
    historico->idPartida = cantReg + 1;

    fwrite(historico, sizeof(tHistorico), 1, pf);
    fclose(pf);
}

void mostrar_historico(const char* file_name) {

    tHistorico historico;
    FILE* pArchivo = fopen(file_name, "rb");

    if(pArchivo == NULL) {
        printf("\nError al abrir el archivo de %s porque no existe aun\n", file_name);
        return;
    }

    printf("  +---------------------------------------------------------------------------------------------+\n");
    printf("  |                                  HISTORICO PARTIDAS                                         |\n");
    printf("  +---------------------------------------------------------------------------------------------+\n");


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
