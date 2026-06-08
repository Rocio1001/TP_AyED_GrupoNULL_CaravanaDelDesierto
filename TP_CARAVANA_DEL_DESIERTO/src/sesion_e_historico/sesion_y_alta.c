/* ******************************************************************
Hay varias cosas que estoy muy seguro que se pueden optimizar o compactar o hacer de otra manera, pero no estoy seguro de como hacerlo.
lo subo asi para que lo tengan, si quieren tomamos esto, si no, no. Por ahi tenian otra idea en mente de como hacerlo.
Creo que se entiende bastante bien, pero cualquier cosa lo vemos, o lo hacemos de otra forma.
Si vamos con esto lo corregimos y cambiamos lo que haya que cambiar y lo documento.

Una cosa si q seguramente esta mal, son los codigos de ERROR y retorno, hay varios con el mismo numero, no se si puede hacer eso
****************************************************************** */

#include "../../include/sesion_e_historico/sesion_y_alta.h"

int compararJugadores(const void *a, const void *b)
{
    const tJugadorMaestro *j1 = (const tJugadorMaestro *)a;
    const tJugadorMaestro *j2 = (const tJugadorMaestro *)b;

    return strcmp(j1->nombre, j2->nombre);
}

///podria retornar cuantos regsitros cargo, para saber cual sería el siguiente id a asignar (? CONSULTAR, al final lo hice con ftell()/tam
void cargarMaestroJugadores(tArbol *arbol, const char *nombArch)
{
    crearArbol( arbol );

    cargarArchivoBinOrdenadoArbolBinBusq(arbol, nombArch, sizeof(tJugadorMaestro));
}

int generarId(const char *nombArch)
{
    int cantReg;
    FILE* pf;
    pf = fopen(nombArch, "rb");

    if( !pf )
        return 1; //el archivo tdv no existe => primer jugador

    fseek(pf, 0, SEEK_END);

    ///mientras no se borren registros esto funciona bien. Pq si hay 100 el ult ID es 101 si se borra por ejemplo el 20...
    ///...quedan 100 registros de nuevo y el 100 quedaria duplicado
    ///pero que haya visto no dice el TP q se deben poder borrar registros (creo)
    ///cualquier cosa lo cambio
    cantReg = ftell(pf) / sizeof(tJugadorMaestro); //cantidad de registros en el archivo c/u tiene un id de 1 a 0

    fclose(pf);

    return cantReg + 1;
}

int guardarMaestroJugadores (tArbol *arbol, const char* nombArch)
{
    FILE *pf;

    if(!*arbol)
        return SIN_INICIALIZAR; //-3

    pf = fopen(nombArch, "wb");
    if(!pf)
        return ERROR_ARCH; //-1

    crearArchOrdBinArbol(arbol, pf);

    fclose(pf);

    return TODO_OK; //1
}

int registrarJugador(tArbol *arbol, tJugador *jugador, const char *nombArch)
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
    id = generarId(nombArch);
    if( id == -1 )
        return ERROR_ARCH; //-1

    //cargo en las structs
    jugador->id = id;
    strcpy(jugador->nombre, nombre);

    jugadorReg.id = id;
    //el nombre ya queda desde antes

    //guardo el nuevo usuario en el arbol de forma ordenada
    insertarRecArbolBinBusq(arbol, &jugadorReg, sizeof(tJugadorMaestro), compararJugadores);

    return JUGADOR_CREADO; //1
}

int darJugadorDeAlta(tArbol *arbol, tJugador *jugador, const char *nombArch)
{
    int resultado;

    do{
        printf("Ingrese nombre de usuario: ");

        resultado = registrarJugador(arbol, jugador, nombArch);

        if(resultado == JUGADOR_ENCONTRADO)
            printf("El nombre ya existe, elija otro\n\n");

    }while(resultado == JUGADOR_ENCONTRADO);

    return resultado;// 1 = JUGADOR_CREADO o -1 = ERROR_ARCH
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

///iniciar_sesion() seguro que se puede optimizar / compactar más
int iniciar_sesion (tJugador *jugador, const char *nombArchMaestro)
{
    int opcion, resultado;
    char buffer[16];
    tArbol maestroJugadores;

    //cargar maestroJugadores a un ABB
    cargarMaestroJugadores(&maestroJugadores, nombArchMaestro); //carga el archivo de jugadores a un arbol

    do
    {
        mostrar_inicioSesion_1();
        fgets(buffer, sizeof(buffer), stdin);
        opcion = atoi(buffer);

        switch(opcion)
        {
            //Es nuevo
            case 1:
                resultado = darJugadorDeAlta(&maestroJugadores, jugador, nombArchMaestro); // 1 o -1
                if( resultado == JUGADOR_CREADO )
                {
                    //si lo pudo crear, lo intenta guardar, si falla al guardar...
                    if( guardarMaestroJugadores(&maestroJugadores, nombArchMaestro) != TODO_OK )
                    {
                        vaciarArbol(&maestroJugadores);
                        return SESION_ERROR; //-1
                    }
                    //...si sale bien
                    vaciarArbol(&maestroJugadores);
                    return SESION_ALTA; //1
                }
                //si directamente no lo pudo crear...
                vaciarArbol(&maestroJugadores);
                return SESION_ERROR;//-1
                break;

            case 2:
                resultado = cargarJugadorExistente(&maestroJugadores, jugador);
                while( !resultado ) //no se econtro
                {
                    system("cls");
                    mostrar_inicioSesion_2();

                    fgets(buffer, sizeof(buffer), stdin);
                    opcion = atoi(buffer);

                    if(opcion == 1)
                    {
                        resultado = cargarJugadorExistente(&maestroJugadores, jugador);
                    }
                    else if (opcion == 2) ///no se como hacer para compactarla mas, pq esto es lo mismo que case 1:
                    {
                        resultado = darJugadorDeAlta(&maestroJugadores, jugador, nombArchMaestro);
                        if(resultado == JUGADOR_CREADO)
                        {
                            if( guardarMaestroJugadores(&maestroJugadores, nombArchMaestro) != TODO_OK ) //codigo de error de archivo o arbol
                            {
                                vaciarArbol(&maestroJugadores);
                                return SESION_ERROR; //-1
                            }
                            else
                            {
                                vaciarArbol(&maestroJugadores);
                                return SESION_ALTA; //1
                            }
                        }
                        //si directamente no lo pudo crear...
                        vaciarArbol(&maestroJugadores);
                        return SESION_ERROR;//-1
                    }
                    else
                    {
                       printf("  Opcion invalida...\n");
                       system("pause");
                    }

                }

                vaciarArbol(&maestroJugadores);
                return SESION_ALTA;
                break;

            default:
                printf("\n  Opcion invalida. Intente nuevamente.\n");
                printf("  Presione Enter para continuar...");
                while(getchar() != '\n');
                break;
        }

    }while(1);
}
