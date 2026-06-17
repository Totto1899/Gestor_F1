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
#define ERR 0
#define ERR_NOTARC -6

//MACROS DE ARCHIVOS
#define CARGA_PILOTO "pilotos.txt"
#define CARGA_ESCUD "escuderias.txt"
#define ARCH_PILOTO "pilotos.dat"
#define ARCH_ESCUD "escuderias.dat"
#define ARCH_CARRERA "carreras.dat"
#define ARCH_CARRERA2 "carreras2.dat"
#define ARCH_PILOTOBAJAS "pilotos_bajas.dat"
#define ARCH_ESCUDBAJAS "escuderias_bajas.dat"
#define ARCH_CARRERABAJAS "carreras_bajas.dat"

//MACROS DE MENU

#define MENUBASE "\n\t---- GESTOR DE F1 ----\n" \
                 "\tINGRESE LA OPCIÓN DESEADA:\n" \
                 "\t1 - Listar pilotos\n" \
                 "\t2 - Mostrar ranking de pilotos de la temporada\n" \
                 "\t3 - Mostrar pilotos por escudería\n" \
                 "\t4 - Calcular estadísticas de pilotos\n" \
                 "\t5 - Exportar datos a archivos de texto\n" \
                 "\t6 - Dar de baja, alta o modificar\n" \
                 "\t7 - Simular una carrera\n" \
                 "\t8 - Combinar temporadas\n" \
                 "\t0 - Finalizar programa\n" \
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
                        "\t3 - Carrera\n"\
                        "\t4 - Volver al menu\n"\
                        "\t-->"


#define MENU_MODIPILOTO "\n\t----INGRESE QUE CAMPO QUIERE MODIFICAR----\n"\
                        "\t1 - Nombre\n"\
                        "\t2 - Nacionalidad\n"\
                        "\t3 - Id escuderia\n"\
                        "\t4 - Estado\n"\
                        "\t5 - Fecha de nacimiento\n"\
                        "\t6 - Puntos\n"\
                        "\t7 - Volver al menu\n"\
                        "\t-->"

#define MENU_MODIESCUDERIA "\n\t----INGRESE QUE CAMPO QUIERE MODIFICAR----\n"\
                            "\t1 - Codigo\n"\
                            "\t2 - Nombre\n"\
                            "\t3 - Pais\n"\
                            "\t4 - Estado\n"\
                            "\t5 - Volver al menu\n"\
                            "\t-->"

#define MENU_EXPORTACION "\n=== MENU DE EXPORTACION A TXT ===\n" \
                         "1. Exportar Pilotos\n" \
                         "2. Exportar Escuderias\n" \
                         "3. Exportar Carreras\n" \
                         "4. Exportar Bajas de Pilotos\n" \
                         "5. Exportar Bajas de Escuderias\n" \
                         "0. Volver al menu principal\n" \
                         "Elija una opcion: "

#define MENU_ESTADISTICAS "\n=== ESTADISTICAS DEL CAMPEONATO ===\n" \
                          "1. Top 5 pilotos con mas victorias\n" \
                          "2. Piloto con mejor promedio general\n" \
                          "3. Piloto que alcanzo la mejor posicion\n" \
                          "4. Piloto que obtuvo la peor posicion\n" \
                          "0. Volver al menu principal\n" \
                          "Elija una opcion: "


#define OPCIONES_MENU "012345678"
#define OPCIONES_ABM "1234"
#define OPCIONES_ABMARCH "1234"
#define OPCIONES_MENUMODIPILOTO "1234567"
#define OPCIONES_MENUMODIESCUDERIA "12345"


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
    int puntos_acumulados;
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
    int estado;/// 1: activo 0:inactivo
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

///FUNCIONES DE ABM
int funcionesABM(const char* piloto, const char* escu, const char* carrera,int cmp(const void*, const void*));

void ingresarRegPiloto(FILE* pf, size_t tam);
void ingresarRegEscuderia(FILE* pf, size_t tam);
int registrarCarrera(const char* nomArchCar, const char* nomArchPil, int cmp(const void*, const void*));///2

int bajaRegPiloto(FILE* pf, size_t tam, const char* arc);
int bajaRegEscuderia(FILE* pf, size_t tam,const char* arc);
int bajaRegCarrera(FILE* pf, size_t tam,const char* arc);

void modifiEscuderia(FILE* pf, const char* arc, size_t tam);
void modifiPiloto(FILE* pf, const char* arc, size_t tam);

///MENUS
char menuBase(const char* msj, const char* opc);

///FUNCIONALIDADES MINIMAS
int listarPilotos(const char* nomArch, void mostrar(const void*));///1
int actualizarPuntosPiloto(const char* nomArch, size_t id_pil, size_t puntos);///3
int listarPilotosPuntos(const char* nomArch, int cmp(const void*, const void*), void mostrar(const void*));///4
int mostrarPilotoXEscuderia(const char* nomArchPil, const char* nomArchEscu, void mostrar(const void*));///5

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

///PUNTOS ADICIONALES

///SIMULACIÓN DE CARRERA
int simularCarrera(const char* nomArchPil, const char* nomArchCar);
size_t contPilActivos(FILE* pf);
int buscarCarreraId(const char* nomArchCar, int id_buscado);
int* asignarPosiciones(FILE* pf_pil, size_t cant_pil_act);
void asignarPuntos(tCarrera* c, int* vec_pos);
void mostrarResultados(tCarrera* car);
int obtenerSiguienteIdCarrera(const char* nomArch);

///COMBINACION DE TEMPORADAS
int combinarCarreras(const char* temp1, const char* temp2, const char* resultado);
void ingresarNomCarrera(char* nomArch);
int leerCarreraCompleta(FILE* pf, tCarrera* car);
int escribirCarreraCompleta(FILE* pf, tCarrera* car);

///BUSQUEDA
int buscarEnArchivo(const char* nomArch, const void* clave, void* destino, size_t tam, int cmp(const void*, const void*));
tPiloto busquedaPiloto(FILE *pf, int clave, size_t tam);
tEscuderia busquedaEscuderia(FILE* pf, int clave, size_t tam);

///ORDENAMIENTO
void ssort(void* vec, size_t ce, size_t tam, int cmp(const void*, const void*));
void mswap(void* a, void* b, size_t tam);
void* buscar_menor(const void* vec, size_t ce, size_t tam, int cmp(const void*, const void*));

#endif // LIBRARY_H_INCLUDED
