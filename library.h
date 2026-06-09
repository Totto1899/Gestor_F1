#ifndef LIBRARY_H_INCLUDED
#define LIBRARY_H_INCLUDED

//MACROS IMPLEMENTADAS
#define TAM_INICIAL 10
#define MAX_LINEA 256
#define ERR_AP -1
#define ERR_ESCRITURA -2
#define TODO_OK 1
#define ERR_MEM -3
#define ERR_BUSQUEDA -4
#define ERR_ACT -5

//MACROS DE ARCHIVOS
#define CARGA_PILOTO "pilotos.txt"
#define CARGA_ESCUD "escuderias.txt"
#define ARCH_PILOTO "pilotos.dat"
#define ARCH_ESCUD "escuderias.dat"
#define ARCH_CARRERA "carreras.dat"
#define ARCH_BAJAS "bajas.dat"

//MACROS DE MENU

#define MENUBASE "\n\t---- GESTOR DE F1 ----\n" \
                 "\tINGRESE LA OPCIÓN DESEADA:\n" \
                 "\t1 - Listar pilotos\n" \
                 "\t2 - Registrar una carrera\n" \
                 "\t3 - Mostrar ranking de pilotos de la temporada\n" \
                 "\t4 - Mostrar pilotos por escudería\n" \
                 "\t5 - Calcular estadísticas de pilotos\n" \
                 "\t6 - Exportar datos a archivos de texto\n" \
<<<<<<< HEAD
                 "\t7 - Dar de baja, alta o modificar\n" \
                 "\t8 - Finalizar programa\n" \
=======
                 "\t7 - ABM de archivos\n" \
                 "\t8 - Finalizar programa\n"\
>>>>>>> 855eb74070c08f4a9be46883ec3cabfa30e0d154
                 "\t--> "

#define MENUABM "\n\t----INGRESE LA ACCIONES QUE QUIERE REALIZAR ----\n"\
                "\t1 - Ingresar un registro\n"\
                "\t2 - Dar de baja un registro\n"\
                "\t3 - Modificar un registro\n"\
                "\t4 - Volver para atras\n"\
                "\t-->"

#define MENUABMARCHIVOS "\n\t----INGRESE SOBRE QUE ARCHIVO ----\n"\
                        "\t1 - Pilotos\n"\
                        "\t2 - Escuderias\n"\
                        "\t3 - Volver al menu\n"\
                        "\t-->"

#define MENU_EXPORTACION "\n=== MENU DE EXPORTACION A TXT ===\n" \
                         "1. Exportar Pilotos\n" \
                         "2. Exportar Escuderias\n" \
                         "3. Exportar Carreras\n" \
                         "4. Exportar Bajas\n" \
                         "0. Volver al menu principal\n" \
                         "Elija una opcion: "

#define MENU_ESTADISTICAS "\n=== ESTADISTICAS DEL CAMPEONATO ===\n" \
                          "1. Top 5 pilotos con mas victorias\n" \
                          "2. Piloto con mejor promedio general\n" \
                          "3. Piloto que alcanzo la mejor posicion\n" \
                          "4. Piloto que obtuvo la peor posicion\n" \
                          "0. Volver al menu principal\n" \
                          "Elija una opcion: "

<<<<<<< HEAD
#define MENU_ABM "\n=== GESTION DE ARCHIVOS (ABM) ===\n" \
                 "1. Gestionar Pilotos\n" \
                 "2. Gestionar Escuderias\n" \
                 "3. Gestionar Carreras\n" \
                 "0. Volver al Menu Principal\n\n" \
                 "Ingrese una opcion: "

#define MENU_ABM_PILOTOS "\n--- ABM PILOTOS ---\n" \
                         "1. Alta de Piloto\n" \
                         "2. Baja Logica de Piloto\n" \
                         "3. Modificar Piloto\n" \
                         "0. Volver al Menu Anterior\n\n" \
                         "Ingrese una opcion: "

#define MENU_ABM_ESCUDERIAS "\n--- ABM ESCUDERIAS ---\n" \
                            "1. Alta de Escuderia\n" \
                            "2. Baja Logica de Escuderia\n" \
                            "3. Modificar Escuderia\n" \
                            "0. Volver al Menu Anterior\n\n" \
                            "Ingrese una opcion: "
#define MENU_ABM_CARRERAS "\n--- ABM CARRERAS ---\n" \
                          "1. Alta de Carrera (Ingresar resultados)\n" \
                          "2. Baja Logica de Carrera (Invalidar)\n" \
                          "3. Modificar Carrera\n" \
                          "0. Volver al Menu Anterior\n\n" \
                          "Ingrese una opcion: "

#define OPCIONES_MENU "12345678"
=======
#define OPCIONES_MENU "12345678"
#define OPCIONES_ABM "1234"
#define OPCIONES_ABMARCH "123"
>>>>>>> 855eb74070c08f4a9be46883ec3cabfa30e0d154

//BIBLIOTECAS UTILIZADAS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<ctype.h>

//TDAs
typedef struct{
    int id_piloto;
    char nombre[30];
    int cant_carreras_corridas;
    int victorias;
    int mejor_posicion;
    int peor_posicion;
    int suma_posiciones;
    float promedio_posicion; // suma_posiciones/cant_carreras_corridas
}tEstadisticaPiloto;

typedef struct{
    unsigned int id_piloto;
    unsigned int posicion;
    unsigned int total_puntos;
}tResultado; //estado 0 invalida la carrera y no se tiene en cuenta (*matriz queda apuntando a NULL)

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
    tResultado* matriz;
}tCarrera;

///FUNCIONES GENERALES
int generarLoteEscuderiasTXT(const char* nomArch);
int generarLotePilotosTXT(const char* nomArch);
int generarLoteArchivoCarrera(const char* nomArchCar, const char* nomArchPil);
void* cargaInicial(const char* nomArch, void* vec, size_t tam, int* capacidad,
                   int* cant_registros, int trozarCamposLongVariable(void*, size_t, const char*));
int volcarABinario(const char* nomArch, const void* vec, size_t tam, size_t ce);
int contElementos(const char* nomArch, size_t tam);

///MENUS
char menuBase(const char* msj, const char* opc);

///FUNCIONALIDADES MINIMAS
int funcionesABM(const char* piloto, const char* escu, const char* carrera);
void ingresarRegPiloto(FILE* pf);
void ingresarRegEscuderia(FILE* pf);
void ingresarRegCarrera(FILE* pf);
void bajaRegPiloto(FILE* pf, size_t tam,const char* arc,int cmp(const void*, const void*));
int cmpPilotorPorId(const void* clave, const void* registro);


int listarPilotos(const char* nomArch, void mostrar(const void*));///1

int registrarCarrera(const char* nomArchCar, const char* nomArchPil, int cmp(const void*, const void*));///2

int actualizarPuntosPiloto(const char* nomArch, size_t id_pil, size_t puntos);///3

int listarPilotosPuntos(const char* nomArch, int cmp(const void*, const void*), void mostrar(const void*));///4

int mostrarPilotoXEscuderia(const char* nomArchPil, const char* nomArchEscu, void mostrarPiloto(const void*));///5

///6
int menuEstadisticas();
tEstadisticaPiloto* generarEstadisticas(size_t* ce);
int obtenerIndicePiloto(tEstadisticaPiloto* vec, size_t cant, int id_buscado);
int cmpVictorias(const void* a, const void* b);
void mostrarTop5(tEstadisticaPiloto* vec, size_t ce);
void mostrarMejorPromedio(tEstadisticaPiloto* vec, size_t cant);
void mostrarMejorPosicion(tEstadisticaPiloto* vec, size_t cant);
void mostrarPeorPosicion(tEstadisticaPiloto* vec, size_t cant);

///7
void menuExportarATXT();
int exportarATXT(const char* nomArchBin, const char* nomArchTXT, size_t tam, void grabar(const void*, FILE*));
void grabarPilotoTXT(const void* registro, FILE* pfTXT);
void grabarEscuderiaTXT(const void* registro, FILE* pfTXT);
int exportarCarreraATXT(const char* nomArchBin, const char* nomArchTXT);

///AMB
void menuABM();
void menuABMPilotos();
void menuABMEscuderias();
void menuABMCarreras();

///BUSQUEDA
void* mbsearch(const void* clave, const void* vec, size_t ce, size_t tam, int cmp(const void*, const void*));
int buscarEnArchivo(const char* nomArch, const void* clave, void* destino, size_t tam, int cmp(const void*, const void*));

///ORDENAMIENTO
void ssort(void* vec, size_t ce, size_t tam, int cmp(const void*, const void*));
void mswap(void* a, void* b, size_t tam);
void* buscar_menor(const void* vec, size_t ce, size_t tam, int cmp(const void*, const void*));

///PROCESAMIENTO
void* mmap(void* vec, size_t ce, size_t tam, void action(void*));
int filter(void* vec, size_t ce, size_t tam, int fred(const void*, size_t* ce));
void reduce(void* vec, size_t ce, size_t tam, void* d, int fred(void*, const void*));
void* mi_memcpy(void* destino, const void* origen, size_t n);
void* mi_memmove(void* destino, const void* origen, size_t n);

#endif // LIBRARY_H_INCLUDED
