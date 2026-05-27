#ifndef LIBRARY_H_INCLUDED
#define LIBRARY_H_INCLUDED

//BIBLIOTECAS UTILIZADAS
#include<stdio.h>
#include<stdlib.h>

//TDAs
typedef struct{
    unsigned id;
    char nombre[30];
    char nacionalidad[30];
    unsigned id_escuderia;
    unsigned puntos_acumulados;
    char estado;
    unsigned long long fechaNacimiento;
}tPiloto;

typedef struct{
    unsigned id;
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
///    int mat_resultados[][];
}tCarrera;

#endif // LIBRARY_H_INCLUDED
