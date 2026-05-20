#ifndef LIBRARY_H_INCLUDED
#define LIBRARY_H_INCLUDED

//BIBLIOTECAS UTILIZADAS
#include<stdio.h>
#include<stdlib.h>

//TDAs
typedef struct{
    unsigned id;
    char[30]nombre;
    char[30]nacionalidad;
    unsigned id_escuderia;
    unsigned puntos_acumulados;
    char estado;
    unsigned long long fechaNacimiento;
}tPiloto;

typedef struct{
    unsigned id;
    char[4]codigo;
    char[30]nombre;
    char[50]pais;
    int estado;
}tEscuderia;

typedef struct{
    int id;
    char[20]circuito;
    unsigned long long fecha;
    int estado;
    int cant_resultados;
    int [][]mat_resultados;
}tCarrera;

#endif // LIBRARY_H_INCLUDED
