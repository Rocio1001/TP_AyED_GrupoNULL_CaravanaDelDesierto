//TODAS las funciones que reciben un FILE *pf por parametro, lo reciben en modo lectura "rb"

#include "../../include/sesion_e_historico/sesion_y_alta.h"

int compararJugadores(const void *a, const void *b)
{
    const tJugadorMaestro *j1 = (const tJugadorMaestro *)a;
    const tJugadorMaestro *j2 = (const tJugadorMaestro *)b;

    return strcmp(j1->nombre, j2->nombre);
}

void cargarMaestroJugadores(tArbol *arbol, FILE *pf)
{
    crearArbol( arbol );

    cargarArchivoBinOrdenadoAbiertoArbolBinBusq(arbol, pf, sizeof(tJugadorMaestro)); ///cargarlo normal no asi
}

//devuelve el puntero al inicio
int generarId(FILE *pf)
{
    int cantReg;

    if( !pf )
        return 1; //el archivo tdv no existe => es primer jugador

    fseek(pf, 0, SEEK_END);

    cantReg = ftell(pf) / sizeof(tJugadorMaestro); //cantidad de registros en el archivo

    rewind(pf); //devuelve el inicio

    return cantReg + 1;
}

//abre el archivo en wb y guarda el arbol de jugadoresMaestro en orden en el archivo
int guardarMaestroJugadores (tArbol *arbol, const char* nombArch)
{
    FILE *pf;

    if(!*arbol)
        return SIN_INICIALIZAR; //-3

    pf = fopen(nombArch, "wb");
    if(!pf)
        return ERROR_ARCH; //-1

    recorrerEnOrdenRecArbolBinBusq(arbol, 0, pf, escribirEnArchivo);

    fclose(pf);

    return TODO_OK; //1
}

//recibe el puntero a maestro en modo lectura solo para generar el nuevo ID
//inserta el nuevo jugador en el arbol de tJugadoresMaestro
//para que este sea guarde despues en el archivo maestro_jugadores.bin
int registrarJugador(tArbol *arbol, tJugador *jugador, FILE *pf)
{
    int id;
    char nombre[30];
    tJugadorMaestro jugadorReg;

    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    strcpy(jugadorReg.nombre, nombre);

    if ( buscarNodoArbolBinBusq(arbol, &jugadorReg, compararJugadores) != NULL )
        return JUGADOR_ENCONTRADO; //2

    //generar Id
    id = generarId(pf);

    //cargo en las structs
    jugador->id = id;
    strcpy(jugador->nombre, nombre);

    jugadorReg.id = id;
    //el nombre ya queda desde antes

    //guardo el nuevo usuario en el arbol de forma ordenada
    if( insertarRecArbolBinBusq(arbol, &jugadorReg, sizeof(tJugadorMaestro), compararJugadores) != TODO_OK ) //puede ser SIN_MEM
        return SIN_MEM; //-4

    return JUGADOR_CREADO; //1
}


int darJugadorDeAlta(tArbol *arbol, tJugador *jugador, FILE *pf)
{
    int resultado;

    do{
        printf("Ingrese nombre de usuario: ");

        resultado = registrarJugador(arbol, jugador, pf);

        if(resultado == JUGADOR_ENCONTRADO)
            printf("El nombre ya existe, elija otro\n\n");

    }while(resultado == JUGADOR_ENCONTRADO);

    return resultado;// 1 = JUGADOR_CREADO o -4 = SIN_MEM para nuevo nodo de tJugadorMaestro en arbol de jugadoresMaestro
}


int cargarJugadorExistente(tArbol *arbol, tJugador *jugador)
{
    char nombre[30];
    tNodoA **ppRes;
    tJugadorMaestro jugadorAux;
    tJugadorMaestro *reg;

    printf("Ingrese nombre de usuario: ");

    fgets(nombre, sizeof(nombre), stdin);
    nombre[strcspn(nombre, "\n")] = '\0';

    strcpy(jugadorAux.nombre, nombre);

    ppRes = buscarNodoArbolBinBusq(arbol, &jugadorAux, compararJugadores);

    if( ppRes == NULL )
        return JUGADOR_NO_ENCONTRADO; //0

    //si != NULL es pq lo encontro =>

    reg = (tJugadorMaestro*)(*ppRes)->info;

    jugador->id = reg->id;
    strcpy(jugador->nombre, reg->nombre);

    return JUGADOR_ENCONTRADO; //1
}

int altaYGuardar (tArbol *arbol, tJugador *jugador, FILE **pMaestro, const char *nombArch)
{
    if( darJugadorDeAlta(arbol, jugador, *pMaestro) != JUGADOR_CREADO )
        return SESION_ERROR; //-2

    if( *pMaestro ) ///(funciona) pero no se si podemos usar FILE**, pero si no iniciar_sesion queda gigante y con codigo repetido
    {
        fclose( *pMaestro );
        *pMaestro = NULL;
    }

    if(guardarMaestroJugadores(arbol, nombArch) != TODO_OK )
        return SESION_ERROR;

    return SESION_ALTA;
}

///iniciar_sesion() seguro que se puede optimizar / compactar más
int iniciar_sesion (tJugador *jugador, const char *nombArchMaestro)
{
    int opcion, resultado, valorRetorno, salir;
    char buffer[16];
    tArbol maestroJugadores;
    FILE *pMaestro;

    pMaestro = fopen(nombArchMaestro, "rb");

    valorRetorno = SESION_ERROR; ///valor por defecto
    salir = 0;

    //si hay archivo cargar maestroJugadores a un ABB
    if(pMaestro)
        cargarMaestroJugadores(&maestroJugadores, pMaestro);
    //si no hay archivo lo crea (y el jugador que se de alta va a ser el primero de todos)
    else
        crearArbol(&maestroJugadores);

    do{
        mostrar_inicioSesion_1();
        fgets(buffer, sizeof(buffer), stdin);
        opcion = atoi(buffer);

        switch(opcion)
        {
            //Es nuevo
            case 1:
                valorRetorno = altaYGuardar(&maestroJugadores, jugador, &pMaestro, nombArchMaestro);
                salir = 1;
                break;

            case 2:
                resultado = cargarJugadorExistente(&maestroJugadores, jugador); //se busca
                //no se econtro
                while( resultado == JUGADOR_NO_ENCONTRADO) //== 0
                {
                    system("cls");

                    mostrar_inicioSesion_2();
                    fgets(buffer, sizeof(buffer), stdin);
                    opcion = atoi(buffer);

                    if(opcion == 1)
                    {
                        resultado = cargarJugadorExistente(&maestroJugadores, jugador); //se busca, si encuentra =1 si no =0
                    }
                    else if (opcion == 2) //darse de alta (igual a case 1)
                    {
                        valorRetorno = altaYGuardar(&maestroJugadores, jugador, &pMaestro, nombArchMaestro);
                        salir = 1;
                        break; //corta el while
                    }
                    else
                    {
                       printf("  Opcion invalida...\n");
                       system("pause");
                    }
                }//fin de while
                if(resultado)
                {
                    valorRetorno = SESION_ALTA;
                    salir = 1;
                }
                break;

            default:
                printf("\n  Opcion invalida. Intente nuevamente.\n");
                printf("  Presione Enter para continuar...");
                while(getchar() != '\n');
                break;
        }
    }while( !salir );

    if(pMaestro)
        fclose(pMaestro);

    vaciarArbol(&maestroJugadores);
    return valorRetorno;
}
