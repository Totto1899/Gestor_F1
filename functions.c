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

int generarLoteArchivoCarrera(const char* nomArchCar, const char* nomArchPil){
    unsigned int puntos_f1[10] = {25, 18, 15, 12, 10, 8, 6, 4, 2, 1};
    int i;
    int ce_pil = contElementos(nomArchPil, sizeof(tPiloto));
    tPiloto* vecPil = malloc(ce_pil*sizeof(tPiloto));
    if(!vecPil)
        return ERR_MEM;
    FILE* pfPil = fopen(nomArchPil, "rb");
    if(!pfPil){
        free(vecPil);
        return ERR_AP;
    }
    fread(vecPil, sizeof(tPiloto), ce_pil, pfPil);
    fclose(pfPil);
    FILE* pfCar = fopen(nomArchCar, "wb");
    if(!pfCar){
        free(vecPil);
        return ERR_AP;
    }
    tCarrera car_aux;
    car_aux.id = 1;
    strcpy(car_aux.circuito, "Monza");
    car_aux.fecha = (unsigned long long)time(NULL);
    car_aux.estado = 1;
    car_aux.cant_resultados = ce_pil;
    car_aux.matriz = malloc(ce_pil*sizeof(tResultado));
    if(!car_aux.matriz){
        free(vecPil);
        fclose(pfCar);
        return ERR_MEM;
    }
    for(i=0; i<ce_pil; i++){
        car_aux.matriz[i].id_piloto = vecPil[i].id;
        car_aux.matriz[i].posicion = i+1;
        if(i<10)
            car_aux.matriz[i].total_puntos = puntos_f1[i];
        else
            car_aux.matriz[i].total_puntos = 0;
    }
    fwrite(&car_aux, sizeof(tCarrera), 1, pfCar);
    fwrite(car_aux.matriz, sizeof(tResultado), ce_pil, pfCar);
    free(vecPil);
    free(car_aux.matriz);
    fclose(pfCar);
    return TODO_OK;
}

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

void* mmap(void* vec, size_t ce, size_t tam, void action(void*)){
    void* r = vec;
    int i;
    for(i=0; i<ce ; i++){
        action(vec);
        vec+=tam;
    }
    return r;
}

int filter(void* vec, size_t ce, size_t tam, int ffilter(const void*, size_t* ce)){
    void* end = vec+(ce*tam);
    void* pr = vec;
    void* pw = vec;
    int cant_filtrada = 0;
    while(pr<end){
        if(ffilter(pr, &ce)){
            if(pr!=pw)
                memmove(pw, pr, tam);
            pw += tam;
        }
        else
            cant_filtrada++;
        pr += tam;
    }
    return cant_filtrada;
}

void reduce(void* vec, size_t ce, size_t tam, void* d, int fred(void*, const void*)){
    int i;
    for(i=0; i<ce ; i++){
        fred(d, vec);
        vec+=tam;
    }
}

void* mi_memcpy(void* destino, const void* origen, size_t n){
    char* end = (char*)origen+n;
    char* temp_d = (char*)destino;
    const char* temp_o = (const char*)origen;
    while(temp_o<end){
        *temp_d = *temp_o;
        temp_o++;
        temp_d++;
    }
    return destino;
}

void* mi_memmove(void* destino, const void* origen, size_t n){
    if(n==0)
        return destino;
    if(destino<origen){
        char* end = (char*)origen+n;
        char* temp_d = (char*)destino;
        const char* temp_o = (const char*)origen;
        while(temp_o<end){
            *temp_d = *temp_o;
            temp_o++;
            temp_d++;
        }
    }
    else{
        char* beg = (char*)origen;
        char* temp_d = (char*)destino+n-1;
        const char* temp_o = (const char*)origen+n-1;
        while(temp_o>=beg){
            *temp_d = *temp_o;
            temp_d--;
            temp_o--;
        }
    }
    return destino;
}

int contElementos(const char* nomArch, size_t tam){
    FILE* pf = fopen(nomArch, "rb");
    if(!pf)
        return ERR_AP;
    fseek(pf, 0, SEEK_END);
    long bytes_totales = ftell(pf);
    fclose(pf);
    return (size_t)(bytes_totales/tam);
}

char menuBase(const char* msj, const char* opc){
    char op;
    do{
        printf("%s",msj);
        fflush(stdin);
        scanf(" %c",&op);
    }while(strchr(opc,op)==NULL);
    return op;
}

///FUNCIONALIDADES MINIMAS

int listarPilotosPuntos(const char* nomArch, int cmp(const void*, const void*), void mostrar(const void*)){
    tPiloto* vec;
    size_t ce;
    int i;
    FILE* pf = fopen(nomArch, "rb");
    if(!pf)
        return ERR_AP;
    ce = contElementos(nomArch, sizeof(tPiloto));
    vec = malloc(ce*sizeof(tPiloto));
    if(!vec){
        fclose(pf);
        return ERR_MEM;
    }
    fread(vec, sizeof(tPiloto), ce, pf);
    ssort(vec, ce, sizeof(tPiloto), cmp);
    for(i=0; i<ce; i++)
        mostrar(vec+i);
    free(vec);
    fclose(pf);
    return TODO_OK;
}

int exportarATXT(const char* nomArchBin, const char* nomArchTXT, size_t tam, void grabar(const char*, FILE*)){
    char linea[MAX_LINEA];
    FILE* pfBin = fopen(nomArchBin, "rb");
    if(!pfBin)
        return ERR_AP;
    FILE* pfTXT = fopen(nomArchTXT, "wt");
    if(!pfTXT){
        fclose(pfBin);
        return ERR_AP;
    }
    fread(linea, tam, 1, pfBin);
    while(!feof(pfBin)){
        grabar(linea, pfTXT);
        fread(linea, tam, 1, pfBin);
    }
    fclose(pfBin);
    fclose(pfTXT);
    return TODO_OK;
}
