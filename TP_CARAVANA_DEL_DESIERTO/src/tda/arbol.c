#include "../../include/tda/arbol.h"


///*************************************************
///*                   CREACIÓN                    *
///*************************************************
void crearArbol(tArbol *pa){
    *pa = NULL;
}
///=============================================///



///*************************************************
///*                DESTRUCCIÓN                    *
///*************************************************
void vaciarArbol(tArbol *pa){
    ///recorro en pos orden, condición de corte -> fin de arbol *pa == NULL

    if(!*pa)
        return;

    ///izq
    vaciarArbol(&(*pa)->izq);
    ///der
    vaciarArbol(&(*pa)->der);
    ///raiz
    free((*pa)->info); //libero info
    free(*pa); //libero el nodo completo
    *pa= NULL;
}

///==============================================///



///*************************************************
///*                INSERCIÓN BST                  *
///*************************************************
//recursivo
int insertarRecArbolBinBusq(tArbol *pa, const void* pd, size_t tam, int(*cmp)(const void*, const void*)){
    tNodoA *nue;
    int resCmp;

    if(*pa){
        if((resCmp=cmp(pd,(*pa)->info))<0)
            return insertarRecArbolBinBusq(&(*pa)->izq,pd,tam,cmp);
        else if(resCmp>0)
            return insertarRecArbolBinBusq(&(*pa)->der,pd,tam,cmp);
        return CLA_DUP;
    }

    nue=malloc(sizeof(tNodoA));
    if(!nue)
        return SIN_MEM;
    nue->info = malloc(tam);
    if(!nue->info){
        free(nue);
        return SIN_MEM;
    }

    memcpy(nue->info,pd, tam);
    nue->tamInfo = tam;
    nue->izq = NULL;
    nue->der = NULL;

    *pa = nue;
    return TODO_OK;
}
///==============================================///



///*************************************************
///*            BUSCAR NODO BST                    *
///*************************************************
//iterativo
tNodoA** buscarNodoArbolBinBusq(tArbol *pa, void* pd,int(*cmp)(const void*, const void*)){
    int resCmp;

    while(*pa && (resCmp = cmp(pd,(*pa)->info)) ){
        if(resCmp<0)
            pa  = &(*pa)->izq;
        else if(resCmp > 0)
            pa  = &(*pa)->der;
    }
    if(!*pa)
        return NULL;

    return (tNodoA**)pa;
}
///=============================================///



///*************************************************
///*            BUSCAR ELEMENTO BST                *
///*************************************************
int buscarElemArbolBinBusq(tArbol *pa, void * pd, unsigned tam,int(*cmp)(const void*,const void*)){

    //podemos llamar a la función de busqueda iterativa o recursiva, es indistinto
    if(!(pa= buscarNodoArbolBinBusq(pa,pd,cmp))) //primero busco al nodo, deposito la dirección de memoria en "pa"
        return 0; /// NO_EXISTE

    memcpy(pd, (*pa)->info,MIN(tam,(*pa)->tamInfo));

    return TODO_OK;
}
///=============================================///

///*************************************************
///*               RECORRIDOS                      *
///*************************************************



void recorrerEnOrdenRecArbolBinBusq(tArbol *pa,size_t nivel,void* params, void(*accion)(void*,size_t,size_t,void*)){
    if(!*pa)
        return;
    /*I*/recorrerEnOrdenRecArbolBinBusq(&(*pa)->izq,nivel+1,params,accion);
    /*R*/accion((*pa)->info,(*pa)->tamInfo,nivel,params);
    /*D*/recorrerEnOrdenRecArbolBinBusq(&(*pa)->der,nivel+1,params,accion);
}
void recorrerEnOrdenArbolBinBusq(tArbol *pa,void* params, void(*accion)(void*,size_t,size_t,void*)){ ///función "envoltorio", llama a la recursiva de arriba
    recorrerEnOrdenRecArbolBinBusq(pa,0,params,accion);
}
///*************************************************
///*               ALTURA, MAXIMOS Y MÍNIMOS       *
///*************************************************

unsigned alturaArbolBin(const tArbol *pa){
    unsigned hi, hd; //hi -> altura izq, hd-> altura derecha
    if(!*pa) //cuando llega a un nodo hoja,retorno 0
        return 0;
    hi = alturaArbolBin(&(*pa)->izq); //calculo la altura del subarbol izq
    hd = alturaArbolBin(&(*pa)->der); //calculo la altura del subarbol der
    return (hi>hd ? hi : hd) + 1; //retorno la altura mayor + 1 (por la altura del nodo raiz)
}
tNodoA** mayorNodoArbolBinBusq(const tArbol *pa){
    if(!*pa)
        return NULL;
    while((*pa)->der)
        pa = &(*pa)->der;
    return (tNodoA**)pa;
}
tNodoA** menorNodoArbolBinBusq(const tArbol *pa){
    if(!*pa)
        return NULL;
    while((*pa)->izq)
        pa = &(*pa)->izq;
    return (tNodoA**)pa;
}

///*************************************************
///*                  ELIMINAR                     *
///*************************************************

int eliminarRaizArbolBinBusq(tArbol *pa){
    tNodoA **remp, *elim;

    if(!*pa)
        return 0;

    free((*pa)->info);
    if(!(*pa)->izq && !(*pa)->der)
    {
        free(*pa);
        *pa = NULL;
        return 1;///OK
    }

    remp = alturaArbolBin(&(*pa)->izq) > alturaArbolBin(&(*pa)->der)?
            mayorNodoArbolBinBusq(&(*pa)->izq):
            menorNodoArbolBinBusq(&(*pa)->der);

    elim = *remp;
    (*pa)->info = elim->info;
    (*pa)->tamInfo = elim->tamInfo;

    *remp = elim->izq ? elim->izq : elim->der;

    free(elim);

    return 1;///OK
}
int eliminarElemArbolBinBusq(tArbol *pa, void *pd, size_t tam,int(*cmp)(const void*,const void*)){
    if(!(pa=buscarNodoArbolBinBusq(pa,pd,cmp)))
        return 0; /// NO_EXISTE

    memcpy(pd,(*pa)->info,MIN(tam,(*pa)->tamInfo));
    return eliminarRaizArbolBinBusq(pa);
}
///=============================================///

///***********************************************************
///* CARGAR ARBOL DESDE ARCHIVOS / FUENTES EXTERNAS ORDENADAS*
///***********************************************************
/*función genérica que crea/arma un arbol balanceado desde un data set (ds) ordenado*/
int cargarDesdeDatosOrdenadosRec(tArbol *pa,//puntero al arbol a construir
                                void *ds, //dataset origen de datos ordenados (puede ser FILE*
                                unsigned (*leer)(void **, void*, unsigned, void * params),//operacion de lectura, cualquiera que se use, DEBE PEDIR MEMORIA PARA LA INFORMACIÓN DEL NODO, y copiar el registro "pos", además de de devolver tam!=0
                                int li,//limite inferior
                                int ls,//limite superior
                                void* params)//parametro auxiliar, tipicamente es "tamInfo" == sizeof(tEstructura).
                                {
    int m = (li+ls)/2; //calculo el medio (m va a ser la raíz del subarbol actual)
    int r;
    if(li>ls) //caso base, rango li>ls, no hay nodo que crear
        return TODO_OK;

    (*pa) = (tNodoA*) malloc(sizeof(tNodoA)); //resermo memoria para el nodo

    if(!*pa || !((*pa)->tamInfo = leer(&(*pa)->info,ds,m,params)))//leer reserva memoria para el dato y lee el archivo, si alguna de las dos falla, retorna 0
    {
        free(*pa);
        return SIN_MEM;
    }

    (*pa)->izq = (*pa)->der = NULL; //inicializo enlaces del nodo como hoja

    if((r=cargarDesdeDatosOrdenadosRec(&(*pa)->izq,ds,leer,li,m-1,params))!=TODO_OK) //construyo el subarbol izq recursivamente
        return r;
    return cargarDesdeDatosOrdenadosRec(&(*pa)->der,ds,leer,m+1,ls,params);//construyo el subarbol der recursivamente
}
int cargarDesdeDatosOrdenadosArbolBinBusq(tArbol * p, void * ds, unsigned cantReg,
                                          unsigned (*leer)(void**,void*,unsigned,void*params),void* params)
                                          {//funcion envoltorio, le mandan ya todos los datos y no debe calcular nada más que llamar a la recursiva
    if(*p||!ds)
        return 0;
    return cargarDesdeDatosOrdenadosRec(p,ds,leer,0,cantReg-1,params);
}
///en caso de que el data set que posee los datos ordenados sea un archivo binario, podemos usar lo siguiente:
unsigned leerDesdeArchivoBin(void ** pd, void * pf,unsigned pos, void * params){
    unsigned tam = *((int*)params); //extrae params, es el tamaño del registro a leer (suele ser una estructura de datos). Primero casteo a entero y luego accedo con la referencia "*" de la izq
    *pd = malloc(tam); //pido memoria para el tamaño del registro a leer
    if(!*pd) //en caso de error de malloc, retorno 0
        return 0;
    fseek((FILE*)pf,pos*tam,SEEK_SET); //me posiciono en el archivo, en la posición pos*tam, desde el inicio
    if(!fread(*pd,tam,1,(FILE*)pf))
        return 0;
    return tam; //retorna el tamaño
}
//si el archivo binario fue previamente abierto, usamos lo siguiente. Es una función envoltorio que prepara el llamado a la recursiva
int cargarArchivoBinOrdenadoAbiertoArbolBinBusq(tArbol *pa, FILE * pf, size_t tamInfo){
    int cantReg;

    if(*pa || !pf) //si el arbol tiene algo cargado o el archivo no se abrio con exito, retorno 0
        return 0;
    fseek(pf,0L,SEEK_END);// me posiciono al final del archivo
    cantReg = ftell(pf)/tamInfo; //calculo la cantidad de registro con ftell/tamaño del dato
    return cargarDesdeDatosOrdenadosRec(pa,pf,leerDesdeArchivoBin,0,cantReg-1, &tamInfo);
}

//si el archivo binario no fue abierto, usamos lo siguiente. Es una función envoltorio que prepara el llamado a la recursiva
int cargarArchivoBinOrdenadoArbolBinBusq(tArbol *p, const char * path,unsigned tamInfo){
    int cantReg,r;

    FILE * pf;
    if(*p)
        return SIN_INICIALIZAR;
    if(!(pf = fopen(path, "rb")))
        return ERROR_ARCH;
    fseek(pf, 0L,SEEK_END);
    cantReg = ftell(pf)/tamInfo;
    r=cargarDesdeDatosOrdenadosRec(p,pf,leerDesdeArchivoBin,0,cantReg - 1,&tamInfo);
    fclose(pf);

    return r;
}

/// AGUS
void crearArchOrdBinArbol(tArbol *p, FILE* pf)
{
    if( !*p )
        return;

    ///RECORRE EL ABB EN ORDEN (MENOR A MAYOR) Y VA GUARDANDO EL EL ARCHIVO BINARIO

    crearArchOrdBinArbol( &(*p)->izq, pf );

    fwrite( (*p)->info, (*p)->tamInfo, 1, pf );

    crearArchOrdBinArbol( &(*p)->der, pf );
}
