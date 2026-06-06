#include "../../include/sistema_de_puntos/ranking.h"

///cmp de inserción
int cmp_id(const void* idA, const void* idB){
    const tRanking *id_a = (const tRanking*)idA;
    const tRanking *id_b = (const tRanking*)idB;
    return (id_a->idJugador - id_b->idJugador);
}
///cmp de ordenamiento
int cmp_criterio_ranking(const void* a, const void* b) {
    const tRanking* rA = (const tRanking*)a;
    const tRanking* rB = (const tRanking*)b;

    // 1er criterio: Mayor cantidad de Puntos va primero (Descendente)
    if (rA->totalPuntos > rB->totalPuntos) return -1;
    if (rA->totalPuntos < rB->totalPuntos) return 1;

    // 2do Criterio (Desempate): Menor cantidad de Partidas (Ascendente)
    if (rA->partidasJugadas < rB->partidasJugadas) return -1;
    if (rA->partidasJugadas > rB->partidasJugadas) return 1;

    //3er criterio (deempate): menor cantidad de movimientos (ascendente)
    if (rA->totalMovimientos < rB->totalMovimientos) return -1;
    if (rA->totalMovimientos > rB->totalMovimientos) return 1;


    // Empate total
    return 0;
}
///accion al insertar
void acumular_ranking(void** datoEnLista, size_t* tamDato, const void* datoAAcumular, size_t tamDatoAcumular){
    // OJO aca: como es un void**, primero hay que desreferenciarlo (*datoEnLista)
    tRanking *jugadorExistente = (tRanking*)(*datoEnLista);
    const tRanking *jugadorRepetido = (const tRanking*)datoAAcumular;

    jugadorExistente->totalPuntos += jugadorRepetido->totalPuntos;
    jugadorExistente->partidasJugadas += jugadorRepetido->partidasJugadas;
    jugadorExistente->totalMovimientos += jugadorRepetido->totalMovimientos;
}
///acción al recorrer ranking
void imprimir_ranking(const void* dato, size_t tamDato, void* params){
    const tRanking *rank = (const tRanking*)dato;

    printf("ID: %-4d | Jugador: %-20s | Puntos: %-6d | Partidas: %d | Movimientos: %d\n",
           rank->idJugador,
           rank->jugador,
           rank->totalPuntos,
           rank->partidasJugadas,
           rank->totalMovimientos);
}


///******************************************************************************************
/// Evaluar si podemos "dividir" esta función... ¿es necesario?
/*
    Dado que el usuario selecciona "ver ranking" desde el menú de opciones, considero que la lista de ranking debe "nacer" y "morir" en la misma función que lo genera.

    Basado en ese criterio, es la razón por la cual la lista del ranking nace y muere en esta función

    Claramente, puede cambiarse y dividirse, a evaluar.
*/
///******************************************************************************************

///creacion, visualización y destrucción del ranking
void crear_y_mostrar_ranking(const char *pathHistorico){
    tLista listaRanking;
    tRanking rank;
    //tHistorico;
    FILE* pf;
    char linea[256];

    pf = fopen(pathHistorico, "rt");
    if(!pf){
        printf("Error al leer archivo %s\n",pathHistorico);
        return;
    }

    crear_lista(&listaRanking);
    while(fgets(linea,sizeof(linea),pf)){
        sscanf(linea,"%d|%29[^|]|%d|%d",
               &rank.idJugador,
               rank.jugador,
               &rank.totalPuntos,
               &rank.totalMovimientos);

        rank.partidasJugadas = 1;
        insertar_ordenado_lista(&listaRanking,&rank,sizeof(tRanking),0,cmp_id,acumular_ranking); //insertamos ordenado por idJugador
    }
    fclose(pf);

    ordenar_lista(&listaRanking,cmp_criterio_ranking); //una vez insertado todos los elementos de forma ordenada por idJugador, ordenamos la lista con el criterior del ranking
    printf("\n============= RANKING TOP JUGADORES =============\n");
    recorrer_lista(&listaRanking,imprimir_ranking,NULL);
    printf("=================================================\n\n");

    vaciar_lista(&listaRanking);

}


