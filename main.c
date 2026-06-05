#include "library.h"

int trozarPilotos(void* vec, size_t tam, const char* linea);
int trozarEscuderia(void* vec, size_t tam, const char* linea);

int main(){
    generarLoteEscuderiasTXT(CARGA_ESCUD);
    generarLotePilotosTXT(CARGA_PILOTO);

    printf("--- INICIANDO MIGRACION DE DATOS F1 ---\n\n");

    printf("[*] Procesando Pilotos...\n");
    int cap_pilotos = TAM_INICIAL;
    int ce_pilotos = 0;
    tPiloto* vectorPilotos = malloc(cap_pilotos * sizeof(tPiloto));
    if (!vectorPilotos){
        printf(ERR_MEM);
        return -1;
    }
    vectorPilotos = cargaInicial(CARGA_PILOTO, vectorPilotos, sizeof(tPiloto), &cap_pilotos,&ce_pilotos, trozarPilotos);
    volcarABinario(ARCH_PILOTO, vectorPilotos, sizeof(tPiloto), ce_pilotos);
    free(vectorPilotos);
    printf("\n");

    printf("[*] Procesando Escuderias...\n");
    int cap_escu = TAM_INICIAL;
    int ce_escu = 0;
    tEscuderia* vectorEscuderias = malloc(cap_escu * sizeof(tEscuderia));
    if (!vectorEscuderias){
        printf(ERR_MEM);
        return -1;
    }
    vectorEscuderias = cargaInicial(CARGA_ESCUD, vectorEscuderias, sizeof(tEscuderia), &cap_escu, &ce_escu, trozarEscuderia);
    volcarABinario(ARCH_ESCUD, vectorEscuderias, sizeof(tEscuderia), ce_escu);
    free(vectorEscuderias);
    printf("\n");

    printf("[*] Creando carrera...\n");
    ///solo una carrera
///    generarLoteArchivoCarrera(ARCH_CARRERA);


    printf("--- MIGRACION FINALIZADA ---\n");
    char op=plantillaMenu(MENUBASE,"123456");

    while(op!='6')
    {
        mandarFunciones(op);
        op=plantillaMenu(MENUBASE,"123456");
    }

    printf("\n ----GRACIAS POR EJECUTAR----");
    return 0;
}

int trozarPilotos(void* vec, size_t tam, const char* linea){
    tPiloto* p = (tPiloto*)vec;
    char linea_copia[MAX_LINEA];
    char* aux;
    strcpy(linea_copia, linea);
    aux = strchr(linea_copia, '\n');
    *aux = '\0';
    aux = strrchr(linea_copia, '|');
    *aux = '\0';
    sscanf(aux+1, "%llu", &p->fechaNacimiento);
    aux = strrchr(linea_copia, '|');
    *aux = '\0';
    sscanf(aux+1, "%c", &p->estado);
    aux = strrchr(linea_copia, '|');
    *aux = '\0';
    sscanf(aux+1, "%u", &p->puntos_acumulados);
    aux = strrchr(linea_copia, '|');
    *aux = '\0';
    sscanf(aux+1, "%u", &p->id_escuderia);
    aux = strrchr(linea_copia, '|');
    *aux = '\0';
    strcpy(p->nacionalidad, aux + 1);
    aux = strrchr(linea_copia, '|');
    *aux = '\0';
    strcpy(p->nombre, aux + 1);
    sscanf(linea_copia, "%u", &p->id);
    return TODO_OK;
}

int trozarEscuderia(void* vec, size_t tam, const char* linea){
    tEscuderia* e = (tEscuderia*)vec;
    char linea_copia[MAX_LINEA];
    char* aux;
    strcpy(linea_copia, linea);
    aux = strchr(linea_copia, '\n');
    aux = strrchr(linea_copia, '|');
    *aux = '\0';
    sscanf(aux + 1, "%d", &e->estado);
    aux = strrchr(linea_copia, '|');
    *aux = '\0';
    strcpy(e->pais, aux + 1);
    aux = strrchr(linea_copia, '|');
    *aux = '\0';
    strcpy(e->nombre, aux + 1);
    aux = strrchr(linea_copia, '|');
    *aux = '\0';
    strcpy(e->codigo, aux + 1);
    sscanf(linea_copia, "%u", &e->id);
    return TODO_OK;
}

void mostrarPiloto(void* aux)
{
    tPiloto* v = (tPiloto*)aux;
    printf(" %s %d", v->nombre, v->puntos_acumulados);
}

