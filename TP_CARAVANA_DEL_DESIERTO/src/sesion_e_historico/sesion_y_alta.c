//TODAS las funciones que reciben un FILE *pf por parametro, lo reciben en modo lectura "rb"

#include "../../include/sesion_e_historico/sesion_y_alta.h"

int cmp_jugadores(const void *a, const void *b){
    const tIndice *j1 = (const tIndice *)a;
    const tIndice *j2 = (const tIndice *)b;

    return strcmp(j1->clave.nombre, j2->clave.nombre);
}

///***************************************************************
void cargar_arbol_indice_maestro_jugadores(tArbol *arbol, FILE **pf){
    tJugadorMaestro jugadorAux;
    tIndice indice;
    size_t pos;

    crear_arbol( arbol );
    rewind(*pf); //just buenas practica

    fread(&jugadorAux,sizeof(tJugadorMaestro),1,*pf);
    pos = 0;
    while(!feof(*pf)){
        strcpy(indice.clave.nombre,jugadorAux.nombre);
        indice.valor = pos;

        insertar_rec_arbol(arbol,&indice,sizeof(tIndice),cmp_jugadores);
        pos++;
        fread(&jugadorAux,sizeof(tJugadorMaestro),1,*pf);
    }
    //el archivo se cierra luego de su llamado
}

int posicion_final_en_archivo(const char *nombArch){
    int cantReg = 0;
    FILE *pf;


    pf = fopen(nombArch, "rb");

    if(!pf)
        return 0; // El archivo no existe => es el primer jugador

    fseek(pf, 0, SEEK_END);
    cantReg = ftell(pf) / sizeof(tJugadorMaestro); // Cantidad de registros

    fclose(pf);
    return cantReg;
}

void buscar_en_maestro(tJugadorMaestro *jugador, tIndice *indice, const char *nombArch){
    FILE *pf ;

    pf= fopen(nombArch, "rb");

    if (!pf) {
        printf("Error al abrir el archivo maestro\n");
        return;
    }
    fseek(pf, indice->valor * sizeof(tJugadorMaestro), SEEK_SET);
    fread(jugador, sizeof(tJugadorMaestro), 1, pf);
    fclose(pf);
}

///
int cargar_jugador_existente(tArbol *arbol, tJugador *jugador, const char *nombArch){
    char nombre[30];
    tNodoA **ppRes;
    tJugadorMaestro jugadorAux;
    tIndice *indiceEncontrado;
    tIndice indice;

    printf("Ingrese nombre de usuario: ");
    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    strcpy(indice.clave.nombre, nombre);

    ppRes = buscar_nodo_arbol(arbol, &indice, cmp_jugadores);

    if( ppRes == NULL )
        return JUGADOR_NO_ENCONTRADO; //0, no existe en el arbol
    ///si != NULL es pq lo encontro =>


    // Recuperamos el indice completo que está en el árbol
    indiceEncontrado = (tIndice*)((*ppRes)->info);

   // Buscamos y recuperamos en el archivo maestro
    buscar_en_maestro(&jugadorAux, indiceEncontrado, nombArch);


    jugador->id = jugadorAux.id;
    strcpy(jugador->nombre, jugadorAux.nombre);

    return JUGADOR_ENCONTRADO; //1
}
///*********************************************
int registar_jugador(tArbol *arbol, tJugadorMaestro *jugador, const char *nombArch){
    char nombre[30];
    tIndice indice;

    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';
    strcpy(indice.clave.nombre, nombre);

    if ( buscar_nodo_arbol(arbol, &indice, cmp_jugadores) != NULL )
        return JUGADOR_ENCONTRADO; // 2

    // Calculamos posición abriendo el archivo atómicamente
    indice.valor = posicion_final_en_archivo(nombArch);

    jugador->id = indice.valor + 1; // ID = POS+1
    strcpy(jugador->nombre, nombre);

    if( insertar_rec_arbol(arbol, &indice, sizeof(tIndice), cmp_jugadores) != TODO_OK )
        return SIN_MEM; //-4

    return JUGADOR_CREADO; // 1
}

int dar_alta_jugador_arbol(tArbol *arbol, tJugadorMaestro *jugador, const char *nombArch){
    int resultado;
    do{
        printf("Ingrese nombre de usuario: ");
        resultado = registar_jugador(arbol, jugador, nombArch);

        if(resultado == JUGADOR_ENCONTRADO){
            printf("El nombre ya existe, elija otro\n\n");
            printf("  Presione Enter para continuar...");
            while(getchar() != '\n');
        }

    }while(resultado == JUGADOR_ENCONTRADO);

    return resultado;
}

int dar_alta_jugador_maestro(const char* pathMaestro, tJugadorMaestro *jugador){
    // "ab" garantiza crearlo si no existe, o poner el puntero al final si existe
    FILE *pf;
    pf = fopen(pathMaestro, "ab");
    if(!pf){
        printf("Hubo problemas al abrir el archivo %s\n", pathMaestro);
        return ERROR_ARCH;
    }

    // la apertura "ab" garantiza ecribir al final, por defecto
    fwrite(jugador, sizeof(tJugadorMaestro), 1, pf);
    fclose(pf);

    return SESION_ALTA;
}

int alta_jugador (tArbol *arbol, tJugadorMaestro *jugador, const char *nombArch){
    // Registra e inserta en el Árbol
    if( dar_alta_jugador_arbol(arbol, jugador, nombArch) != JUGADOR_CREADO )
        return SESION_ERROR; //-2

    // Persiste en el archivo Maestro
    if( dar_alta_jugador_maestro(nombArch, jugador) != SESION_ALTA )
        return SESION_ERROR;

    return SESION_ALTA;
}

///envoltorio
int procesar_alta_y_mapeo(tArbol *arbol, tJugador *jugador, tJugadorMaestro *jugadorSesion, const char *nombArch) {
    int valorRetorno;

    valorRetorno = alta_jugador(arbol, jugadorSesion, nombArch);

    if (valorRetorno == SESION_ALTA) {
        jugador->id = jugadorSesion->id;
        strcpy(jugador->nombre, jugadorSesion->nombre);
    }
    return valorRetorno;
}


///iniciar_sesion(), orquestador
int iniciar_sesion (tJugador *jugador, const char *nombArchMaestro){
    int opcion, resultado, valorRetorno, salir;
    char buffer[16];
    tArbol arbolIndiceJugadores;
    FILE *pMaestro;
    tJugadorMaestro jugadorSesion;

    valorRetorno = SESION_ERROR; ///valor por defecto
    salir = 0;

    // === CARGA INICIAL ATÓMICA ===
    pMaestro = fopen(nombArchMaestro, "rb");
    if(pMaestro) { //si hay archivo, cargo el ABB
        cargar_arbol_indice_maestro_jugadores(&arbolIndiceJugadores, &pMaestro);
        fclose(pMaestro); // cerramos
    } else {//si no hay archivo lo crea (y el jugador que se de alta va a ser el primero de todos)
        crear_arbol(&arbolIndiceJugadores);
    }


    do{
        system("cls");
        mostrar_inicioSesion_1();
        fgets(buffer, sizeof(buffer), stdin);
        opcion = atoi(buffer);

        switch(opcion)
        {
            /// -----ES NUEVO-----
                case 1:
                    valorRetorno = procesar_alta_y_mapeo(&arbolIndiceJugadores, jugador, &jugadorSesion, nombArchMaestro);
                    salir = 1;
                break;
            /// -----JUGADOR YA TIENE USUARIO-----
                case 2:
                    resultado = cargar_jugador_existente(&arbolIndiceJugadores, jugador, nombArchMaestro);

                    // MIENTRAS NO SE ENCUENTRE EL JUGADOR
                    while( resultado == JUGADOR_NO_ENCONTRADO )
                    {
                        system("cls");
                        mostrar_inicioSesion_2(); // Submenú de error
                        fgets(buffer, sizeof(buffer), stdin);
                        opcion = atoi(buffer);

                        if(opcion == 1) {
                            //  Reintentar
                            resultado = cargar_jugador_existente(&arbolIndiceJugadores, jugador, nombArchMaestro);
                        }
                        else if (opcion == 2) {
                            //  Darse de alta, para no repetir codigo del case 1, llamamos a una envoltorio
                            valorRetorno = procesar_alta_y_mapeo(&arbolIndiceJugadores, jugador, &jugadorSesion, nombArchMaestro);
                            salir = 1;
                            resultado = JUGADOR_ENCONTRADO;
                        }
                        else {
                           printf("  Opcion invalida...\n");
                           system("pause");
                        }
                    }
                // Si el while terminó porque lo encontró en un reintento (resultado == JUGADOR_ENCONTRADO)
                    if(resultado == JUGADOR_ENCONTRADO) {
                        valorRetorno = SESION_ALTA;
                        salir = 1;
                    }
                break;
            case 3:
                /// --- SALIR ---
                    system("cls");
                    printf("  Hasta la proxima!\n\n");
                break;


            default:
                    printf("\n  Opcion invalida. Intente nuevamente.\n");
                    printf("  Presione Enter para continuar...");
                    while(getchar() != '\n');
                break;
        }
    }while( opcion != 3 && !salir );

    vaciar_arbol(&arbolIndiceJugadores);
    return valorRetorno;
}
