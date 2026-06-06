#include "../../include/juego/tablero.h"

/* =========================================================
   CREACION Y DESTRUCCION
   ========================================================= */

void crear_tablero(tTablero *tablero){
    crear_lista_doble_cir(&tablero->lista);
    tablero->cantidadCasilleros = 0;
}
void destruir_tablero(tTablero *tablero){
    vaciar_lista_doble_cir(&tablero->lista);
    tablero->cantidadCasilleros = 0;
}

/* =========================================================
   CONFIGURACION
   ========================================================= */

int leer_configuracion(const char* ruta_archivo, tConfig* config){
    FILE* archConfig;
    char linea[MAX_LINEA];

    archConfig = fopen(ruta_archivo, "rt");
    if(!archConfig){
        printf("Error: No se pudo abrir el archivo %s\n", ruta_archivo);
        return CONFIG_ERROR;
    }

    while(fgets(linea, sizeof(linea), archConfig)){
        if(strstr(linea, "cantidad_posiciones"))
            sscanf(linea, "%*[^:]: %d", &config->cantidadPosiciones);
        else if(strstr(linea, "vidas_inicio"))
            sscanf(linea, "%*[^:]: %d", &config->vidasInicio);
        else if(strstr(linea, "maximo_bandidos"))
            sscanf(linea, "%*[^:]: %d", &config->maxBandidos);
        else if(strstr(linea, "maximo_premios"))
            sscanf(linea, "%*[^:]: %d", &config->maxPremios);
        else if(strstr(linea, "maximo_vidas_extra"))
            sscanf(linea, "%*[^:]: %d", &config->maxVidasExtras);
        else if(strstr(linea, "maximo_oasis"))
            sscanf(linea, "%*[^:]: %d", &config->maxOasis);
        else if(strstr(linea, "maximo_tormentas"))
            sscanf(linea, "%*[^:]: %d", &config->maxTormentas);
    }

    fclose(archConfig);
    return CONFIG_OK;
}

int validar_configuracion(const tConfig* config){
    int casillerosIntermedios, elementosTotal, elementosHostiles;
    float hostilidad;

    if(config->cantidadPosiciones < 10 ||
       config->vidasInicio < 1 ||
       config->maxBandidos < 0 ||
       config->maxPremios < 0 ||
       config->maxVidasExtras < 0 ||
       config->maxOasis < 0 ||
       config->maxTormentas < 0){
        printf("Error: Valores negativos o tablero demasiado pequeño (min 10).\n");
        return CONFIG_ERROR;
    }

    casillerosIntermedios = config->cantidadPosiciones - 2;

    elementosTotal = config->maxOasis + config->maxTormentas +
                     config->maxVidasExtras + config->maxPremios;

    // Con MAX_ELEMENTOS_ESTATICOS por casillero, caben el doble de slots
    if(elementosTotal > casillerosIntermedios * MAX_ELEMENTOS_ESTATICOS){
        printf("Error: Imposible colocar %d elementos en %d casilleros (max %d por casillero).\n",
               elementosTotal, casillerosIntermedios, MAX_ELEMENTOS_ESTATICOS);
        return CONFIG_ERROR;
    }

    elementosHostiles = config->maxBandidos + config->maxTormentas;
    hostilidad = (float)elementosHostiles / casillerosIntermedios;
    if(hostilidad > MAX_HOSTILIDAD_PERMITIDA){
        printf("Error: Escenario injugable. Hostilidad: %.0f%% (Max %.0f%%).\n",
               hostilidad * 100, MAX_HOSTILIDAD_PERMITIDA * 100);
        return CONFIG_ERROR;
    }

    return CONFIG_OK;
}

int preparar_configuracion(const char* ruta_archivo, tConfig* config){
    if(leer_configuracion(ruta_archivo, config) == CONFIG_ERROR){
        system("pause");
        return CONFIG_ERROR;
    }
    if(validar_configuracion(config) == CONFIG_ERROR){
        system("pause");
        return CONFIG_ERROR;
    }
    return CONFIG_OK;
}

/* =========================================================
   CONSULTAS SOBRE CASILLEROS
   ========================================================= */

int casillero_es_inicio(const tCasillero* cas){
    int i;
    for(i = 0; i < cas->cantEstaticos; i++)
        if(cas->estaticos[i] == CAS_INICIO) return 1;
    return 0;
}

int casillero_es_fin(const tCasillero* cas){
    int i;
    for(i = 0; i < cas->cantEstaticos; i++)
        if(cas->estaticos[i] == CAS_FIN) return 1;
    return 0;
}

/*
 * Verifica si se puede agregar un tipo estatico al casillero.
 * Restricciones:
 *   - No superar MAX_ELEMENTOS_ESTATICOS
 *   - No puede haber dos OASIS
 *   - No puede haber dos TORMENTAS
 *   - No puede haber OASIS + TORMENTA juntos
 */
int puede_agregar_estatico(const tCasillero* cas, tTipoCasillero tipo){
    int i;
    if(cas->cantEstaticos >= MAX_ELEMENTOS_ESTATICOS) return 0;
    for(i = 0; i < cas->cantEstaticos; i++){
        if(tipo == CAS_OASIS    && cas->estaticos[i] == CAS_OASIS)    return 0;
        if(tipo == CAS_TORMENTA && cas->estaticos[i] == CAS_TORMENTA) return 0;
        if(tipo == CAS_OASIS    && cas->estaticos[i] == CAS_TORMENTA) return 0;
        if(tipo == CAS_TORMENTA && cas->estaticos[i] == CAS_OASIS)    return 0;
    }
    return 1;
}

/*
 * Elimina la primera aparicion de `tipo` del arreglo de estaticos
 * del casillero, compactando el array.
 */
void remover_estatico_casillero(tCasillero* cas, tTipoCasillero tipo){
    int i, j;
    for(i = 0; i < cas->cantEstaticos; i++){
        if(cas->estaticos[i] == tipo){
            for(j = i; j < cas->cantEstaticos - 1; j++)
                cas->estaticos[j] = cas->estaticos[j + 1];
            cas->cantEstaticos--;
            return;
        }
    }
}

/*
 * Reposiciona un bandido desde su casillero actual a uno intermedio
 * aleatorio valido (sin superar MAX_BANDIDOS_POR_CASILLERO).
 * Se usa cuando el bandido queda atrapado en INICIO al volver el jugador.
 */
void reposicionar_bandido(tTablero* tablero, tBandido* bandido){
    tNodoD* actual;
    tCasillero* cas;
    int intentos = 0, j, posAleat;

    /* Liberar el slot en el casillero actual */
    cas = (tCasillero*)bandido->posicion->info;
    cas->cantBandidos--;

    do {
        posAleat = (rand() % (tablero->cantidadCasilleros - 2)) + 1;
        actual = tablero->lista;
        for(j = 0; j < posAleat; j++)
            actual = actual->sig;
        cas = (tCasillero*)actual->info;
        intentos++;
    } while(cas->cantBandidos >= MAX_BANDIDOS_POR_CASILLERO && intentos < 50);

    bandido->posicion = actual;
    cas->cantBandidos++;
}

/* =========================================================
   CONSTRUCCION DEL TABLERO
   ========================================================= */

/*
 * Distribuye los elementos estaticos en los casilleros intermedios.
 * Algoritmo:
 *   1. Inserta INICIO, N-2 casilleros vacios y FIN en la lista.
 *   2. Genera un vector con todos los elementos a colocar y lo mezcla.
 *   3. Para cada elemento elige un casillero intermedio aleatorio
 *      y verifica las restricciones antes de insertarlo (max 200 intentos).
 */
int construir_casilleros_estaticos_tablero(tTablero* tablero, const tConfig* config){
    int i, j, totalElementos, colocado, intentos, posAleat;
    tCasillero casillero;
    tNodoD* nodoActual;
    tTipoCasillero* elementos;
    tTipoCasillero temp;

    // --- Paso 1: insertar todos los casilleros en la lista ---

    casillero.cantEstaticos = 1;
    casillero.estaticos[0]  = CAS_INICIO;
    casillero.hayJugador    = 0;
    casillero.cantBandidos  = 0;
    if(insertar_ult_lista_doble_cir(&tablero->lista, &casillero, sizeof(tCasillero)) != TODO_OK)
        return TABLERO_ERROR;

    casillero.cantEstaticos = 0;
    for(i = 1; i < config->cantidadPosiciones - 1; i++){
        if(insertar_ult_lista_doble_cir(&tablero->lista, &casillero, sizeof(tCasillero)) != TODO_OK)
            return TABLERO_ERROR;
    }

    casillero.cantEstaticos = 1;
    casillero.estaticos[0]  = CAS_FIN;
    casillero.hayJugador    = 0;
    casillero.cantBandidos  = 0;
    if(insertar_ult_lista_doble_cir(&tablero->lista, &casillero, sizeof(tCasillero)) != TODO_OK)
        return TABLERO_ERROR;

    // --- Paso 2: armar y mezclar vector de elementos ---
    totalElementos = config->maxOasis + config->maxTormentas +
                     config->maxVidasExtras + config->maxPremios;

    if(totalElementos == 0) return TODO_OK;

    elementos = (tTipoCasillero*)malloc(sizeof(tTipoCasillero) * totalElementos);
    if(!elementos) return TABLERO_ERROR;

    j = 0;
    for(i = 0; i < config->maxOasis;       i++) elementos[j++] = CAS_OASIS;
    for(i = 0; i < config->maxTormentas;   i++) elementos[j++] = CAS_TORMENTA;
    for(i = 0; i < config->maxVidasExtras; i++) elementos[j++] = CAS_VIDA_EXTRA;
    for(i = 0; i < config->maxPremios;     i++) elementos[j++] = CAS_PREMIO;

    // Fisher-Yates shuffle
    for(i = totalElementos - 1; i > 0; i--){
        j = rand() % (i + 1);
        temp = elementos[i];
        elementos[i] = elementos[j];
        elementos[j] = temp;
    }

    // --- Paso 3: distribuir cada elemento en un casillero intermedio valido ---
    for(i = 0; i < totalElementos; i++){
        colocado = 0;
        intentos = 0;
        while(!colocado && intentos < 200){
            posAleat = (rand() % (config->cantidadPosiciones - 2)) + 1;

            nodoActual = tablero->lista;
            for(j = 0; j < posAleat; j++)
                nodoActual = nodoActual->sig;

            if(puede_agregar_estatico((tCasillero*)nodoActual->info, elementos[i])){
                tCasillero* cas = (tCasillero*)nodoActual->info;
                cas->estaticos[cas->cantEstaticos++] = elementos[i];
                colocado = 1;
            }
            intentos++;
        }
        if(!colocado){
            free(elementos);
            printf("Error: no se pudieron colocar todos los elementos estaticos.\n");
            return TABLERO_ERROR;
        }
    }

    free(elementos);
    return TODO_OK;
}

void posicionar_e_inicializar_jugador(tTablero* tablero, tJugador *jugador, const tConfig *config){
    tCasillero* cas;

    jugador->posicion = tablero->lista;
    cas = (tCasillero*)jugador->posicion->info;
    cas->hayJugador = 1;

    jugador->id          = 0; ///Revisar
    jugador->nombre[0]   = '\0'; ///Revisar
    jugador->vidas       = config->vidasInicio;
    jugador->proteccion  = 0;
    jugador->turnoValido = 1;
    jugador->puntos      = 0;
}

int posicionar_bandidos(tTablero* tablero, int totalBandidos, tBandido *bandido){
    int i, j, intentos, posAleatoria;
    tNodoD* casilleroActual;
    tCasillero* info;

    for(i = 0; i < totalBandidos; i++){
        intentos = 0;
        do {
            posAleatoria = (rand() % (tablero->cantidadCasilleros - 2)) + 1;
            casilleroActual = tablero->lista;
            for(j = 0; j < posAleatoria; j++)
                casilleroActual = casilleroActual->sig;
            info = (tCasillero*)casilleroActual->info;
            intentos++;
        } while(info->cantBandidos >= MAX_BANDIDOS_POR_CASILLERO && intentos < 50);

        info->cantBandidos++;
        bandido[i].id       = i;
        bandido[i].posicion = casilleroActual;
    }
    return TODO_OK;
}

int construir_tablero(tTablero *tablero, tBandido **bandido, tJugador *jugador, const tConfig *config){
    tablero->cantidadCasilleros = config->cantidadPosiciones;

    *bandido = (tBandido*)malloc(sizeof(tBandido) * config->maxBandidos);
    if(!*bandido){
        printf("Sin memoria.\n");
        return TABLERO_ERROR;
    }

    if(construir_casilleros_estaticos_tablero(tablero, config) != TODO_OK){
        printf("Error al generar el tablero.\n");
        return TABLERO_ERROR;
    }

    if(posicionar_bandidos(tablero, config->maxBandidos, *bandido) != TODO_OK){
        printf("Error al posicionar bandidos.\n");
        return TABLERO_ERROR;
    }

    posicionar_e_inicializar_jugador(tablero, jugador, config);
    return TABLERO_GENERADO;
}

/* =========================================================
   VISUALIZACION
   ========================================================= */

static char char_de_estatico(tTipoCasillero tipo){
    switch(tipo){
        case CAS_INICIO:     return 'I';
        case CAS_FIN:        return 'F';
        case CAS_OASIS:      return 'O';
        case CAS_TORMENTA:   return 'T';
        case CAS_VIDA_EXTRA: return 'V';
        case CAS_PREMIO:     return 'P';
        default:             return '?';
    }
}

void actualizar_caravana_txt(tTablero *tablero, const char *pathCaravanaTxt){
    FILE *pf;
    tNodoD *nodo_actual;
    tCasillero *cas;
    int i, j;

    pf = fopen(pathCaravanaTxt, "wt");
    if(!pf){
        printf("Error: No se pudo crear/abrir el archivo %s\n", pathCaravanaTxt);
        return;
    }

    if(!tablero->lista){
        fprintf(pf, "El tablero esta vacio.\n");
        fclose(pf);
        return;
    }

    nodo_actual = tablero->lista;
    fprintf(pf, "=== ESTADO DE LA CARAVANA ===\n\n");

    for(i = 0; i < tablero->cantidadCasilleros; i++){
        cas = (tCasillero*)nodo_actual->info;

        fprintf(pf, "[");

        // '-' solo si el casillero no tiene NINGUN elemento (ni estatico ni dinamico)
        if(cas->cantEstaticos == 0 && cas->cantBandidos == 0 && !cas->hayJugador){
            fprintf(pf, "-");
        } else {
            // Elementos estaticos
            for(j = 0; j < cas->cantEstaticos; j++){
                if(j > 0) fprintf(pf, " ");
                fprintf(pf, "%c", char_de_estatico(cas->estaticos[j]));
            }

            // Bandidos (uno por uno)
            for(j = 0; j < cas->cantBandidos; j++){
                if(cas->cantEstaticos > 0 || j > 0) fprintf(pf, " ");
                fprintf(pf, "B");
            }

            // Jugador
            if(cas->hayJugador){
                if(cas->cantEstaticos > 0 || cas->cantBandidos > 0) fprintf(pf, " ");
                fprintf(pf, "J");
            }
        }

        fprintf(pf, "]\n");
        nodo_actual = nodo_actual->sig;
    }

    fprintf(pf, "\n=============================\n");
    fclose(pf);
}

void mostrar_caravana_txt(const char *pathCaravanaTxt){
    FILE *pf;
    char linea[MAX_LINEA];

    pf = fopen(pathCaravanaTxt, "rt");
    if(!pf){
        printf("Error: No se encontro el archivo %s.\n", pathCaravanaTxt);
        return;
    }
    while(fgets(linea, sizeof(linea), pf))
        printf("%s", linea);
    fclose(pf);
}
