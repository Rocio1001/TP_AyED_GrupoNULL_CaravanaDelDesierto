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
