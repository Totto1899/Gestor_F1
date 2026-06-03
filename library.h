#ifndef LIBRARY_H_INCLUDED
#define LIBRARY_H_INCLUDED

//MACROS IMPLEMENTADAS
#define TAM_INICIAL 10
#define MAX_LINEA 256
#define ERR_AP -1
#define ERR_ESCRITURA -2
#define TODO_OK 1
#define ERR_MEM "No se pudo reservar memoria"

//MACROS DE ARCHIVOS
#define CARGA_PILOTO "pilotos.txt"
#define CARGA_ESCUD "escuderias.txt"
#define ARCH_PILOTO "pilotos.dat"
#define ARCH_ESCUD "escuderias.dat"
#define ARCH_CARRERA "carreras.dat"
#define ARCH_BAJAS "bajas.dat"

//MACROS DE MENU
#define MENU "\t----GESTOR DE F1----\n \tINGRESE LO QUE DESEE HACER: \n\t1- VER LOS CORREDORES Y SUS PUNTAJES\n \t2- VER LAS ESCUDERRIAS\n \t3- VER ESTADISTICAS DE PILOTOS\n  \t5- EXPORTAR A ARCHIVOS DE TEXTO\n \t6- FINALIZAR PROGRAMA\n --> "


//BIBLIOTECAS UTILIZADAS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//TDAs
typedef struct{
    unsigned int id;
    char nombre[30];
    char nacionalidad[30];
    unsigned id_escuderia;
    unsigned puntos_acumulados;
    char estado;
    unsigned long long fechaNacimiento;
}tPiloto;

typedef struct{
    unsigned int id;
    char codigo[4];
    char nombre[30];
    char pais[50];
    int estado;
}tEscuderia;

typedef struct{
    int id;
    char circuito[20];
    unsigned long long fecha;
    int estado;
    int cant_resultados;
    int mat_resultados[][2];
}tCarrera;

/// FUNCIONES

int generarLoteEscuderiasTXT(const char* nomArch);
int generarLotePilotosTXT(const char* nomArch);
int generarLoteArchivoCarrera(const char* nom);

void* cargaInicial(const char* nomArch, void* vec, size_t tam, int* capacidad,
                   int* cant_registros, int trozarCamposLongVariable(void*, size_t, const char*));
int volcarABinario(const char* nomArch, const void* vec, size_t tam, size_t ce);
int generarLoteArchivoCarrea(const char* nom);

///MENUS
char menuBase(const char* msj, const char* opc);
void mandarFunciones(const char op);

///MOSTRAR COSAS
void mostrarPilotos();

///BUSQUEDA
void* mbsearch(const void* clave, const void* vec, size_t ce, size_t tam, int cmp(const void*, const void*));
void* buscarSecuencial(void* vec, size_t ce, size_t tam, const void* clave);

///ORDENAMIENTO
void ssort(void* vec, size_t ce, size_t tam, int cmp(const void*, const void*));
void mswap(void* a, void* b, size_t tam);
void* buscar_menor(const void* vec, size_t ce, size_t tam, int cmp(const void*, const void*));


#endif // LIBRARY_H_INCLUDED
