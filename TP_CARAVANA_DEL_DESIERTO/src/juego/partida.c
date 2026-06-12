#include "../../include/juego/partida.h"

/* =========================================================
   INICIALIZACION Y FINALIZACION
   ========================================================= */

int inicializar_partida(tPartida* partida, const char* rutaConfig){
    tConfig config;

    partida->bandidos    = NULL;
    partida->cantBandidos = 0;

    if(preparar_configuracion(rutaConfig, &config) == CONFIG_ERROR)
        return TABLERO_ERROR;

    crear_tablero(&partida->tablero);

    if(construir_tablero(&partida->tablero, &partida->bandidos, &partida->jugador, &config) != TABLERO_GENERADO){
        destruir_tablero(&partida->tablero);
        return TABLERO_ERROR;
    }

    partida->cantBandidos = config.maxBandidos;
    partida->estado_juego = 0;
    partida->numero_turno = 0;
    return TODO_OK;
}

void finalizar_partida(tPartida* partida){
    destruir_tablero(&partida->tablero);
    if(partida->bandidos){
        free(partida->bandidos);
        partida->bandidos = NULL;
    }
}

/* =========================================================
   LOOP PRINCIPAL DE LA PARTIDA
   ========================================================= */

int ejecutar_partida(tPartida* partida){
    tCola cola;
    tCasillero* casillaActual;
    tLogMovimientos entrada;
    FILE* logFile;
    int gano = 0;
    int dado, dir, i;

    crear_cola(&cola);

    /* Crear/resetear el archivo de log al inicio de cada partida */
    logFile = fopen(RUTA_LOG, "w");

    while(partida->estado_juego == 0){
        /* Expirar proteccion si no fue consumida en el turno anterior */
        if(partida->jugador.proteccion > 0)
            partida->jugador.proteccion--;

        system("cls");
        mostrar_estado(partida->numero_turno, &partida->jugador, &partida->tablero);

        entrada.id     = partida->numero_turno + 1;
        entrada.nroMov = partida->numero_turno + 1;

        if(turno_valido(&partida->jugador)){

            // --- 1. El jugador elige su movimiento y lo encola ---
            dado = tirar_dado();
            if(distancia_desde_inicio(&partida->tablero, &partida->jugador) >= dado){
                dir = elegir_direccion();
            } else {
                dir = 1; // no hay espacio para retroceder
                printf("  No hay casilleros suficientes para retroceder. Avanzas obligatoriamente.\n");
                printf("\n  Presione Enter para continuar...");
                while(getchar() != '\n');
            }
            encolar_movimiento(&cola, &partida->jugador, dado, dir);

            // --- 2. Registrar en log: "FX" o "BX" ---
            entrada.descripcion[0] = (dir == 1) ? 'F' : 'B';
            entrada.descripcion[1] = '0' + dado;
            entrada.descripcion[2] = '\0';
            escribir_log(logFile, &entrada);

            // --- 3. Los bandidos calculan y encolan sus movimientos ---
            if(partida->cantBandidos > 0)
                procesar_turno_bandidos(&cola,
                                        partida->bandidos, partida->cantBandidos,
                                        &partida->jugador, &partida->tablero);

            // --- 4. Desencolar y aplicar: primero jugador, luego bandidos ---
            desencolar_y_mover(&cola, &partida->tablero, &gano);
            for(i = 0; i < partida->cantBandidos; i++)
                desencolar_y_mover(&cola, &partida->tablero, &gano);

            // --- 5. Refrescar tablero con la nueva posicion de todos ---
            system("cls");
            mostrar_estado(partida->numero_turno, &partida->jugador, &partida->tablero);

            // --- 6. Verificar encuentro con bandidos ---
            verificar_encuentro_bandidos(&partida->jugador, partida->bandidos, partida->cantBandidos, &partida->tablero);

            // --- 7. Aplicar efectos del casillero donde cayo el jugador ---
            casillaActual = (tCasillero*)partida->jugador.posicion->info;
            aplicar_efectos_casillero(&partida->jugador, casillaActual);

        } else {
            /* Turno perdido por tormenta */
            printf("\n  Turno perdido! La tormenta del turno anterior te impide moverte.\n");
            partida->jugador.turnoValido = 1;

            // Registrar NT en el log
            strcpy(entrada.descripcion, "NT");
            escribir_log(logFile, &entrada);

            // Los bandidos se mueven aunque el jugador este bloqueado
            if(partida->cantBandidos > 0){
                procesar_turno_bandidos(&cola,
                                        partida->bandidos, partida->cantBandidos,
                                        &partida->jugador, &partida->tablero);
                for(i = 0; i < partida->cantBandidos; i++)
                    desencolar_y_mover(&cola, &partida->tablero, &gano);

                system("cls");
                mostrar_estado(partida->numero_turno, &partida->jugador, &partida->tablero);

                // Un bandido pudo haberse movido al casillero del jugador
                verificar_encuentro_bandidos(&partida->jugador, partida->bandidos, partida->cantBandidos, &partida->tablero);
            }
        }

        partida->numero_turno++;

        if(gano){
            partida->estado_juego = 1;
            break;
        }
        if(partida->jugador.vidas <= 0){
            partida->estado_juego = -1;
            break;
        }

        printf("\n  Presione Enter para continuar...");
        while(getchar() != '\n');
    }

    vaciar_cola(&cola);
    if(logFile) fclose(logFile);

    // Pantalla final
    system("cls");
    actualizar_caravana_txt(&partida->tablero, RUTA_CARAVANA);
    mostrar_caravana_txt(RUTA_CARAVANA);

    if(partida->estado_juego == 1)
        printf("\n  GANASTE! Llegaste al final en %d turnos. Puntos: %d\n",
               partida->numero_turno, partida->jugador.puntos);
    else if(partida->estado_juego == -1)
        printf("\n  GAME OVER. Te quedaste sin vidas. Puntos: %d\n",
               partida->jugador.puntos);

    // Mostrar log de la partida
    mostrar_log_partida(RUTA_LOG);

    return partida->estado_juego;
}

void actualizar_vista_juego(tPartida* partida){
    actualizar_caravana_txt(&partida->tablero, RUTA_CARAVANA);
    mostrar_caravana_txt(RUTA_CARAVANA);
}
