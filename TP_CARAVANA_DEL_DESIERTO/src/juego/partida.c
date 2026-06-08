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
   EFECTOS DE CASILLERO
   ========================================================= */

/*
 * Aplica los efectos de los elementos estaticos del casillero sobre el jugador.
 * Premio y Vida Extra desaparecen del tablero al ser recogidos.
 * Oasis otorga proteccion para el SIGUIENTE turno.
 * Tormenta hace perder el SIGUIENTE turno (o consume la proteccion si la tenia).
 */
static void aplicar_efectos_casillero(tJugador* jugador, tCasillero* cas){
    int i;
    /* Recorremos en orden; el array puede achicarse si removemos elementos */
    for(i = 0; i < cas->cantEstaticos; ){
        switch(cas->estaticos[i]){
            case CAS_PREMIO:
                jugador->puntos += 100;
                printf("  Premio! +100 puntos. Total: %d pts.\n", jugador->puntos);
                remover_estatico_casillero(cas, CAS_PREMIO);
                /* No incrementar i: el elemento fue removido, hay uno nuevo en i */
                break;

            case CAS_VIDA_EXTRA:
                jugador->vidas++;
                printf("  Vida extra! Vidas: %d\n", jugador->vidas);
                remover_estatico_casillero(cas, CAS_VIDA_EXTRA);
                break;

            case CAS_OASIS:
                jugador->proteccion = 2; // 2 decrementos: caduca al inicio del turno siguiente al proximo
                printf("  Oasis! Tendras proteccion en el proximo turno.\n");
                i++;
                break;

            case CAS_TORMENTA:
                if(jugador->proteccion){
                    jugador->proteccion = 0;
                    printf("  Tormenta! Tu proteccion te salvo.\n");
                } else {
                    jugador->turnoValido = 0;
                    printf("  Tormenta! Perderas el proximo turno.\n");
                }
                i++;
                break;

            default:
                i++;
                break;
        }
    }
}

/*
 * Verifica si el jugador comparte casillero con algun bandido activo.
 * Si lo hay:
 *   - El bandido es eliminado del tablero.
 *   - Si el jugador tiene proteccion: la consume y NO pierde vida ni vuelve al inicio.
 *   - Si no tiene proteccion: pierde 1 vida y vuelve al INICIO.
 *     Al llegar al inicio se reposicionan los bandidos que hubiera alli.
 */
static void verificar_encuentro_bandidos(tPartida* partida){
    tCasillero* cas = (tCasillero*)partida->jugador.posicion->info;
    tCasillero* casInicio;
    int i, j;

    if(cas->cantBandidos == 0) return;

    /* Buscar el primer bandido activo en el casillero del jugador */
    for(i = 0; i < partida->cantBandidos; i++){
        if(partida->bandidos[i].posicion == partida->jugador.posicion){

            /* Eliminar el bandido del tablero */
            cas->cantBandidos--;
            partida->bandidos[i].posicion = NULL;

            if(partida->jugador.proteccion){
                partida->jugador.proteccion = 0;
                printf("  Un bandido te intercepto! Tu proteccion te salvo (sin perdida de vida).\n");
            } else {
                partida->jugador.vidas--;
                printf("  Un bandido te intercepto! Perdiste 1 vida. Vidas: %d\n",
                       partida->jugador.vidas);

                /* Volver al INICIO */
                cas->hayJugador = 0;
                partida->jugador.posicion = partida->tablero.lista;
                casInicio = (tCasillero*)partida->jugador.posicion->info;
                casInicio->hayJugador = 1;
                printf("  Volviste al inicio.\n");

                /* Reposicionar bandidos que estuvieran en INICIO */
                if(casInicio->cantBandidos > 0){
                    printf("  Habia bandidos en el inicio. Seran reposicionados.\n");
                    for(j = 0; j < partida->cantBandidos; j++){
                        if(partida->bandidos[j].posicion == partida->tablero.lista)
                            reposicionar_bandido(&partida->tablero, &partida->bandidos[j]);
                    }
                }
            }
            break; /* Solo 1 bandido aplica la regla por turno */
        }
    }
}

/* =========================================================
   HELPERS INTERNOS
   ========================================================= */

static void mostrar_estado(tPartida* partida){
    actualizar_caravana_txt(&partida->tablero, RUTA_CARAVANA);
    mostrar_caravana_txt(RUTA_CARAVANA);
    printf("\n  Turno: %-4d | Jugador: %-20s | Vidas: %d | Puntos: %-6d | Proteccion: %s\n",
           partida->numero_turno + 1,
           partida->jugador.nombre,
           partida->jugador.vidas,
           partida->jugador.puntos,
           partida->jugador.proteccion > 0 ? "Si" : "No");
    printf("  -------------------------------------------------------\n");
}

/*
 * Cuenta cuantos casilleros hay entre INICIO y la posicion del jugador
 * avanzando hacia adelante. Si el resultado es menor al dado, el jugador
 * no tiene espacio para retroceder y solo puede ir hacia adelante.
 */
static int distancia_desde_inicio(const tTablero* tablero, const tJugador* jugador){
    tNodoD* act = tablero->lista; /* primer nodo = INICIO */
    int dist = 0;
    while(act != jugador->posicion && dist < tablero->cantidadCasilleros){
        act = act->sig;
        dist++;
    }
    return dist;
}

/* Escribe una entrada en el archivo de log */
static void escribir_log(FILE* logFile, const tLogMovimientos* entrada){
    if(!logFile) return;
    fprintf(logFile, "ID: %-4d | Turno: %-4d | Movimiento: %s\n",
            entrada->id, entrada->nroMov, entrada->descripcion);
}

/* Muestra el contenido del log al finalizar la partida */
static void mostrar_log_partida(const char* ruta){
    FILE* f = fopen(ruta, "r");
    char linea[128];
    if(!f){ printf("\n  (Log no disponible)\n"); return; }
    printf("\n  +-----------------------------------------------+\n");
    printf("  |         LOG DE MOVIMIENTOS DE LA PARTIDA      |\n");
    printf("  +-----------------------------------------------+\n");
    while(fgets(linea, sizeof(linea), f))
        printf("  %s", linea);
    printf("  +-----------------------------------------------+\n");
    fclose(f);
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
        mostrar_estado(partida);

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
            mostrar_estado(partida);

            // --- 6. Verificar encuentro con bandidos ---
            verificar_encuentro_bandidos(partida);

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
                mostrar_estado(partida);

                // Un bandido pudo haberse movido al casillero del jugador
                verificar_encuentro_bandidos(partida);
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
