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
///matriz: el primer numero es el id_piloto y el segundo la cantidad de puntos que gana
/*int generarLoteArchivoCarrera(const char* nom)
{
    FILE* pf = fopen(nom, "wb");
    int ce=2;
    ///0=terminada 1=
    tCarrera vec[]={{1,"Mónaco",01062026,1,10,{{101,12},{102,25},{103,8},{104,15},{105,18},{106,1},{107,4},{108,2},{109,10},{110,6}}},
                    {2,"Japón",02062026,1,10,
                                            {101,0,
                                            102,25,
                                            103,18,
                                            104,0,
                                            105,1,
                                            106,4,
                                            107,0,
                                            108,0,
                                            109,0,
                                            110,0}}};
    if (!pf)
        return ERR_AP;
    fwrite(&vec,sizeof(tCarrera),(sizeof(vec)/sizeof(tCarrera)),pf);
    fclose(pf);

    return TODO_OK;
}
*/

void ssort(void* vec, size_t ce, size_t tam, int cmp(const void*, const void*)){
    void* ult = vec + (ce-1)*tam;
    void* men;
    while(vec<ult){
        men = buscar_menor(vec, ce, tam, cmp);
        mswap(men, vec, tam);
        vec +=tam;
        ce--;
    }
}

void mswap(void* a, void* b, size_t tam){
    int i;
    char temp;
    for(i=0; i<tam; i++){
        temp = *(char*)a;
        *(char*)a = *(char*)b;
        *(char*)b = temp;
        a++;
        b++;
    }
}

void* buscar_menor(const void* vec, size_t ce, size_t tam, int cmp(const void*, const void*)){
    void* men = (void*)vec;
    void* ult = (void*)vec + ce*tam;
    void* ini = (void*)vec;
    while(ini<ult){
        if(cmp(men, ini)>0)
            men = ini;
        ini += tam;
    }
    return men;
}

void* mbsearch(const void* clave, const void* vec, size_t ce, size_t tam,
               int cmp(const void*, const void*)){

    size_t elem_izq;
    void* pm;
    int res;
    while(ce > 0){
        elem_izq = ce / 2;
        pm = (void*)vec + (elem_izq * tam);
        res = cmp(clave, pm);
        if(res==0)
            return pm;
        else if(res > 0){
            vec = pm + tam;
            ce = ce - elem_izq - 1;
        }
        else
            ce = elem_izq;
    }
    return NULL;
}

void mostrarPilotos()
{
    FILE* pf=fopen(ARCH_PILOTO,"rb");
    tPiloto aux;
    if(pf==NULL)
    {
         printf("error");
    }
    fread(&aux,sizeof(tPiloto),1,pf);
    printf("----PILOTOS DE F1----");
    while(!feof(pf))
    {
        printf("\n %s %u", aux.nombre, aux.puntos_acumulados);
        fread(&aux,sizeof(tPiloto),1,pf);
    }
    fclose(pf);
}

char menuBase(const char* msj, const char* opc)
{
    char op;

    do
    {
        printf("%s",msj);
        fflush(stdin);
        scanf("%c",&op);
    }while(strchr(opc,op)==NULL);

    return op;
}

void mandarFunciones(const char op)
{
    if(op=='1')
        mostrarPilotos();

}
