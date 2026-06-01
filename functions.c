#include "library.h"

void* cargaInicial(const char* nomArch, void* vec, size_t tam, int* capacidad,
                   int* cant_registros, int trozarCamposLongVariable(void*, size_t, const char*)){
    char aux[MAX_LINEA];
    *cant_registros = 0;
    char* p_read = (char*)vec;
    FILE* pf = fopen(nomArch, "rt");
    if(!pf)
        return vec;
    while(fgets(aux, sizeof(aux), pf)){
        if(*cant_registros>=*capacidad){
            *capacidad *= 2;
            void* temp = realloc(vec, (*capacidad)*tam);
            if(!temp){
                fclose(pf);
                return vec;
            }
            vec = temp;
            p_read = (char*)vec + (*cant_registros*tam);
        }
        if(trozarCamposLongVariable(p_read, tam, aux)){
        p_read += tam;
        (*cant_registros)++;
        }
    }
    fclose(pf);
    if(cant_registros>0 && *cant_registros<*capacidad){
        vec = realloc(vec, (*cant_registros)*tam);
        *capacidad = *cant_registros;
    }
    return vec;
}

int volcarABinario(const char* nomArch, const void* vec, size_t tam, size_t ce){
    FILE* pf = fopen(nomArch, "wb");
    if(!pf)
        return ERR_AP;
    size_t escritos = fwrite(vec, tam, ce, pf);
    fclose(pf);
    if(escritos!=ce)
        return ERR_ESCRITURA;
    return TODO_OK;
}

int generarLoteEscuderiasTXT(const char* nomArch){
    FILE* pf = fopen(nomArch, "wt");
    if (!pf)
        return ERR_AP;
    fprintf(pf, "1|RBR|Red Bull Racing|Austria|1\n");
    fprintf(pf, "2|FER|Scuderia Ferrari|Italia|1\n");
    fprintf(pf, "3|MER|Mercedes AMG|Alemania|1\n");
    fprintf(pf, "4|MCL|McLaren|Reino Unido|1\n");
    fprintf(pf, "5|AST|Aston Martin|Reino Unido|1\n");
    fprintf(pf, "6|WIL|Williams Racing|Reino Unido|1\n");
    fprintf(pf, "7|ALP|Alpine|Francia|1\n");
    fprintf(pf, "8|MIN|Minardi|Italia|0\n");
    fclose(pf);
    return TODO_OK;
}

int generarLotePilotosTXT(const char* nomArch) {
    FILE* pf = fopen(nomArch, "wt");
    if (!pf)
        return ERR_AP;
    // Red Bull (Escudería 1)
    fprintf(pf, "101|Max Verstappen|Neerlandesa|1|395|A|19970930\n");
    fprintf(pf, "102|Sergio Perez|Mexicana|1|258|A|19900126\n");

    // Ferrari (Escudería 2)
    fprintf(pf, "103|Charles Leclerc|Monegasca|2|280|A|19971016\n");
    fprintf(pf, "104|Carlos Sainz|Espanola|2|200|A|19940901\n");

    // Mercedes (Escudería 3)
    fprintf(pf, "105|Lewis Hamilton|Britanica|3|220|A|19850107\n");
    fprintf(pf, "106|George Russell|Britanica|3|160|A|19980215\n");

    // McLaren (Escudería 4)
    fprintf(pf, "107|Lando Norris|Britanica|4|195|A|19991113\n");
    fprintf(pf, "108|Oscar Piastri|Australiana|4|150|A|20010406\n");

    // Aston Martin (Escudería 5)
    fprintf(pf, "109|Fernando Alonso|Espanola|5|180|A|19810729\n");
    fprintf(pf, "110|Lance Stroll|Canadiense|5|60|A|19981029\n");

    // Casos especiales para probar filtros de estado (Retirados / Suspendidos)
    fprintf(pf, "111|Kimi Raikkonen|Finlandesa|2|0|R|19791017\n");
    fprintf(pf, "112|Kevin Magnussen|Danesa|6|15|S|19921005\n");
    fclose(pf);
    return TODO_OK;
}




