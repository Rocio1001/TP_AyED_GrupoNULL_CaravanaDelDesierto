#include "../../include/juego/tablero.h"

///creacion y destrucción
void crear_tablero(tTablero *tablero){
    crear_lista_doble_cir(&tablero->lista);
}
void destruir_tablero(tTablero *tablero){
    vaciar_lista_doble_cir(&tablero->lista);
}

///validación de juego viable
int validar_configuracion(const tConfig* config) {

    int casillerosIntermedios;
    int elementosSpawnTotal;
    int elementosHostiles;
    float porcentajeHostilidad = 0;

    // Validacion inicial - evitar números negativos
    if (config->cantidadPosiciones<0 ||
        config->cantidadPosiciones <10 ||
        config->vidasInicio < 1 ||
        config->maxBandidos < 0 ||
        config->maxPremios < 0 ||
        config->maxVidasExtras < 0 ||
        config->maxOasis < 0 ||
        config->maxTormentas < 0
        ) {

        printf("Error: Valores negativos o tablero demasiado pequeño (min 10).\n");
        return CONFIG_ERROR;
    }

    casillerosIntermedios = config->cantidadPosiciones - 2;


    // Validar que todos los elementos del tablero, entren en el tablero
    elementosSpawnTotal = config->maxOasis + config->maxTormentas +
                            config->maxVidasExtras + config->maxPremios +
                            config->maxBandidos;

    if (elementosSpawnTotal > casillerosIntermedios) {
        printf("Error: Imposible spawnear %d elementos en %d casilleros libres.\n",
               elementosSpawnTotal, casillerosIntermedios);
        return CONFIG_ERROR;
    }

    // Porcentaje de Jugabilidad
    elementosHostiles = config->maxBandidos + config->maxTormentas;

    // Calculamos qué porcentaje del tablero es puro peligro
    porcentajeHostilidad = (float)elementosHostiles / casillerosIntermedios;

    // Verificamos que no pase el límite (55%) (puede cambiarse en la macro y probar otros %)
    if (porcentajeHostilidad > MAX_HOSTILIDAD_PERMITIDA) {
            printf("Error: Escenario injugable. Hay %.0f%% de hostilidad (Max %.0f%% permitido).\n",
                     porcentajeHostilidad * 100, MAX_HOSTILIDAD_PERMITIDA * 100);
        return CONFIG_ERROR;
    }

    return CONFIG_OK;
}

///leo config.txt y lo bajo a la estructura
int leer_configuracion(const char* ruta_archivo, tConfig* config) {
    FILE* archConfig;
    char linea[MAX_LINEA];
    //char* dosPuntos;
    //int valor;

    archConfig = fopen(ruta_archivo, "rt");
    if (!archConfig) {
        printf("Error: No se pudo abrir el archivo %s\n", ruta_archivo);
        return CONFIG_ERROR;
    }

    // Inicializamos la estructura en 0 por seguridad _recomenación de la IA
    //memset(config, 0, sizeof(tConfig));

    // Leemos el archivo línea por línea
    while (fgets(linea, sizeof(linea), archConfig)) {

        // Identificamos el parámetro y extraemos su valor con sscanf
        if (strstr(linea, "cantidad_posiciones")) {
            sscanf(linea, "%*[^:]: %d", &config->cantidadPosiciones);
        }
        else if (strstr(linea, "vidas_inicio")) {
            sscanf(linea, "%*[^:]: %d", &config->vidasInicio);
        }
        else if (strstr(linea, "maximo_bandidos")) {
            sscanf(linea, "%*[^:]: %d", &config->maxBandidos);
        }
        else if (strstr(linea, "maximo_premios")) {
            sscanf(linea, "%*[^:]: %d", &config->maxPremios);
        }
        else if (strstr(linea, "maximo_vidas_extra")) {
            sscanf(linea, "%*[^:]: %d", &config->maxVidasExtras);
        }
        else if (strstr(linea, "maximo_oasis")) {
            sscanf(linea, "%*[^:]: %d", &config->maxOasis);
        }
        else if (strstr(linea, "maximo_tormentas")) {
            sscanf(linea, "%*[^:]: %d", &config->maxTormentas);
        }
    }
    /*
    //OTRA FORMA DE LEERLO
    while (fgets(linea, sizeof(linea), archConfig)) {

        // Buscamos dónde están los dos puntos ':' en la línea leída
        dosPuntos = strchr(linea, ':');

        if (dosPuntos != NULL) {
            // 'dosPuntos' es un puntero al ':'.
            // 'dosPuntos + 1' apunta al número que está inmediatamente después.
            // atoi ignora los espacios en blanco iniciales mágicamente.
            valor = atoi(dosPuntos + 1);

            // Buscamos la palabra clave en la línea ignorando la viñeta
            if (strstr(linea, "cantidad_posiciones")) {
                config->cantidadPosiciones = valor;
            }
            else if (strstr(linea, "vidas_inicio")) {
                config->vidasInicio = valor;
            }
            else if (strstr(linea, "maximo_bandidos")) {
                config->maxBandidos = valor;
            }
            else if (strstr(linea, "maximo_premios")) {
                config->maxPremios = valor;
            }
            else if (strstr(linea, "maximo_vidas_extra")) {
                config->maxVidas_extras = valor;
            }
            else if (strstr(linea, "maximo_oasis")) {
                config->maxOasis = valor;
            }
            else if (strstr(linea, "maximo_tormentas")) {
                config->maxTormentas = valor;
            }
        }
    }
*/



    fclose(archConfig);
    return CONFIG_OK; // CONFIG_OK
}

///construyo y coloco los casilleros en el tablero
int construir_casilleros_estaticos_tablero(tTablero* tablero, const tConfig* config) {
    int i, j, indiceActual;
    tTipoCasillero tempTipo;
    tCasillero casilleroNuevo;
    tTipoCasillero* vectorTipos;

    // Pedimos memoria dinámica para nuestro vector temporal
    vectorTipos = (tTipoCasillero*)malloc(config->cantidadPosiciones * sizeof(tTipoCasillero));
    if (!vectorTipos) {
        return TABLERO_ERROR; // Sin memoria
    }

    // Fijamos los anclajes iniciales
    vectorTipos[0] = CAS_INICIO;
    vectorTipos[config->cantidadPosiciones - 1] = CAS_FIN;

    // Llenamos el vector temporal según el config.txt
    indiceActual = 1;

    for (i = 0; i < config->maxOasis; i++) {
        vectorTipos[indiceActual++] = CAS_OASIS;
    }
    for (i = 0; i < config->maxTormentas; i++) {
        vectorTipos[indiceActual++] = CAS_TORMENTA;
    }
    for (i = 0; i < config->maxVidasExtras; i++) {
        vectorTipos[indiceActual++] = CAS_VIDA_EXTRA;
    }
    for (i = 0; i < config->maxPremios; i++) {
        vectorTipos[indiceActual++] = CAS_PREMIO;
    }

    // Rellenamos los casilleros sobrantes con casilleros vacíos
    while (indiceActual < config->cantidadPosiciones - 1) {
        vectorTipos[indiceActual++] = CAS_VACIO;
    }

    // Mezclamos de forma random los casilleros estaticos
    // Solo mezclamos los elementos INTERMEDIOS (índices del 1 al N-2).
    for (i = config->cantidadPosiciones - 2; i > 1; i--) {
        // Generamos un índice aleatorio 'j' entre 1 e 'i'
        j = (rand() % i) + 1;

        // Intercambiamos el elemento en 'i' con el elemento en 'j'
        tempTipo = vectorTipos[i];
        vectorTipos[i] = vectorTipos[j];
        vectorTipos[j] = tempTipo;
    }

    // Ahora que el vector está mezclado, lo pasamos al tablero oficial (lista circular doble)
    for (i = 0; i < config->cantidadPosiciones; i++) {

        // Armamos el casillero
        casilleroNuevo.tipo = vectorTipos[i];
        casilleroNuevo.hayJugador = 0;   // Se posicionará después
        casilleroNuevo.cantBandidos = 0; // Se posicionarán después

        // Insertamos al final
        if (insertar_ult_lista_doble_cir(&tablero->lista, &casilleroNuevo, sizeof(tCasillero)) != TODO_OK) {
            free(vectorTipos);
            return TABLERO_ERROR; // Falla al insertar
        }
    }

    // Destruimos vector temporal
    free(vectorTipos);

    return TODO_OK;
}
///posiciono a los bandidos y al jugador
void posicionar_e_inicializar_jugador(tTablero* tablero, tJugador *jugador, tConfig *config){
    tCasillero* infoCasillero;

    // El jugador apunta al primer nodo del tablero
    jugador->posicion = tablero->lista;

    //Extraemos la información de ese primer nodo y actualizamos el casillero, avisando que hay un jugador
    infoCasillero = (tCasillero*)jugador->posicion->info;
    infoCasillero->hayJugador = 1;

    // valores iniciales
    jugador->proteccion = 0;
    jugador->turnoValido = 1;
    jugador->puntos = 0;
}
int posicionar_bandidos(tTablero* tablero, int totalBandidos, tBandido *bandido) {
    int i, intentos, posAleatoria;
    tNodoD* casilleroActual;
    tCasillero* info;

    for (i = 0; i < totalBandidos; i++) {
        intentos = 0;

        // Intentamos encontrar un lugar válido con corte de control
        do {
            // Elegir un número aleatorio entre 1 y N-2
            posAleatoria = (rand() % (tablero->cantidadCasilleros - 2)) + 1;

            // Navegar hasta ese nodo
            casilleroActual = tablero->lista; //le doy el inicio de al lista
            for (int j = 0; j < posAleatoria; j++) {
                casilleroActual = casilleroActual->sig;
            }

            info = (tCasillero*)casilleroActual->info;
            intentos++;

        // Condición de éxito: Casillero con menos de X bandidos
        } while (info->cantBandidos >= MAX_BANDIDOS_POR_CASILLERO && intentos < 50);

        // Spawneo
        info->cantBandidos++;
        bandido[i].id = i;
        bandido[i].posicion = casilleroActual;

    }

    return TODO_OK;
}

///construye el tablero, TODO (llama a las funciones que hacen posible la creación de todo el tablero)
int construir_tablero(tTablero *tablero, tBandido **bandido, tJugador *jugador, const char *pathConfigTxt){
    //FILE *pCaravana;
    tConfig configuracion;

    ///leo config.txt
    if(leer_configuracion(pathConfigTxt,&configuracion) == CONFIG_ERROR){
        system("pause"); //acá pongo un "pause" porque el mensaje de "error" ya lo dice la misma funcion que lee, por ende aca solo recibimos y pausamos en caso de error.
        return CONFIG_ERROR;
    }
    ///valido que sea una configuración con una solución posible (que sea jugable)
    if(validar_configuracion(&configuracion) == CONFIG_ERROR){
        system("pause"); //acá pongo un "pause" porque el mensaje de "error" ya lo dice la misma funcion que valida, por ende aca solo recibimos y pausamos en caso de error.
        return CONFIG_ERROR;
    }
    ///ya conocemos cuántos casilleros tiene, asi que lo escribimos
    tablero->cantidadCasilleros = configuracion.cantidadPosiciones;

    ///creo el vector de bandidos (esto se hace acá porque ya validamos que es posible)
    *bandido = (tBandido*)malloc(sizeof(tBandido)*configuracion.maxBandidos);
    if(!*bandido){
        printf("Sin memoria.");
        return TABLERO_ERROR;
    }

    ///distribuyo los casilleros estáticos en el tablero
    if(construir_casilleros_estaticos_tablero(tablero, &configuracion)!= TODO_OK){
        printf("Error al generar el tablero.");
        return TABLERO_ERROR;
    }
    ///distribuyo los bandidos en el tablero y verifico que sea valido
    if(posicionar_bandidos(tablero,configuracion.maxBandidos,*bandido)!=TODO_OK){
        printf("Error al posicionar bandidos.");
        return TABLERO_ERROR;
    }
    ///posiciono al jugador en el tablero (al inicio)
    posicionar_e_inicializar_jugador(tablero,jugador,&configuracion);
    return TABLERO_GENERADO;
}

///escribe caravana.txt
void actualizar_caravana_txt(tTablero *tablero, const char *pathCaravanaTxt) {
    FILE *pf;
    tNodoD *nodo_actual;
    tCasillero *casillero;
    int i;
    char charTerreno;

    pf = fopen(pathCaravanaTxt, "wt");
    if (!pf) {
        printf("Error: No se pudo crear/abrir el archivo %s\n", pathCaravanaTxt);
        return;
    }

    if (!tablero->lista) {
        fprintf(pf, "El tablero está vacío o no se ha generado.\n");
        fclose(pf);
        return;
    }

    nodo_actual = tablero->lista;

    fprintf(pf, "=== ESTADO DE LA CARAVANA ===\n\n");

    for (i = 0; i < tablero->cantidadCasilleros; i++) {
        casillero = (tCasillero*)nodo_actual->info;

        switch (casillero->tipo) {
            case CAS_INICIO:     charTerreno = 'I'; break;
            case CAS_FIN:        charTerreno = 'F'; break;
            case CAS_OASIS:      charTerreno = 'O'; break;
            case CAS_TORMENTA:   charTerreno = 'T'; break;
            case CAS_VIDA_EXTRA: charTerreno = 'V'; break;
            case CAS_PREMIO:     charTerreno = 'P'; break;
            case CAS_VACIO:      charTerreno = '-'; break;
            default:             charTerreno = '?'; break;
        }

        fprintf(pf, "[%c", charTerreno);

        if (casillero->hayJugador) {
            fprintf(pf, " J");
        }
        //por ahora esta así, esto hay que modificarlo
        if (casillero->cantBandidos > 0) {
            if (casillero->cantBandidos > 1) {
                fprintf(pf, " B%d", casillero->cantBandidos);
            } else {
                fprintf(pf, " B");
            }
        }

        // Cerramos el casillero
        fprintf(pf, "]\n");

        nodo_actual = nodo_actual->sig;
    }

    fprintf(pf, "\n\n=============================\n");

    fclose(pf);
}
void mostrar_caravana_txt(const char *pathCaravanaTxt) {
    FILE *pf;
    char linea[MAX_LINEA];

    pf = fopen(pathCaravanaTxt, "rt");
    if (!pf) {
        printf("Error: No se encontro el archivo %s.\n", pathCaravanaTxt);
        return;
    }
    while (fgets(linea, sizeof(linea), pf)) {
        printf("%s", linea);
    }
    fclose(pf);
}
