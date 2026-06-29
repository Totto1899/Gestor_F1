#include "library.h"

int trozarPilotos(void* vec, size_t tam, const char* linea);
int trozarEscuderia(void* vec, size_t tam, const char* linea);

///item 2
int cmpPilotoPorId(const void* clave, const void* registro);

///item 4
int cmpPilotosPorPuntos(const void*, const void*);
void mostrarPiloto(const void* v); ///tambien usada para item 5

int main(){
    generarLoteEscuderiasTXT(CARGA_ESCUD);
    generarLotePilotosTXT(CARGA_PILOTO);

    printf("--- INICIANDO MIGRACION DE DATOS F1 ---\n\n");

    printf("[*] Procesando Pilotos...\n");
    int cap_pilotos = TAM_INICIAL;
    int ce_pilotos = 0;
    tPiloto* vectorPilotos = malloc(cap_pilotos * sizeof(tPiloto));
    if (!vectorPilotos){
        printf("\nNo se pudo reservar memoria para los pilotos");
        return -3;
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
        printf("\nNo se pudo reservar memoria para las escuderias");
        return -3;
    }
    vectorEscuderias = cargaInicial(CARGA_ESCUD, vectorEscuderias, sizeof(tEscuderia), &cap_escu, &ce_escu, trozarEscuderia);
    volcarABinario(ARCH_ESCUD, vectorEscuderias, sizeof(tEscuderia), ce_escu);
    free(vectorEscuderias);
    printf("\n");
    /*
    printf("[*] Creando carrera...\n");
    generarLoteArchivoCarrera(ARCH_CARRERA, ARCH_PILOTO);

    printf("--- MIGRACION FINALIZADA ---\n");

    FILE* pf = fopen(ARCH_CARRERA2, "wb");
    if (pf){
        fclose(pf);
        printf("[OK] El archivo de carreras fue destruido y vaciado a 0 bytes.\n");
        system("pause");
    }
    */
    srand(time(NULL));
    int flag, random;

    char op = 0;

    do{
        system("cls");
        op = menuBase(MENUBASE, OPCIONES_MENU);
        switch(op){
            case '1':
                flag = listarPilotos(ARCH_PILOTO, mostrarPiloto);
                if(flag==-1)
                    printf("\nNo se pudo abrir el archivo de pilotos.dat");
                break;
            case '2':
                flag = listarPilotosPuntos(ARCH_PILOTO, cmpPilotosPorPuntos, mostrarPiloto);
                if(flag==ERR_AP)
                    printf("\nNo se pudo abrir el archivo de pilotos.dat");
                else if(flag==ERR_MEM)
                    printf("\nNo se pudo reservar memoria.");
                break;
            case'3':
                flag = mostrarPilotoXEscuderia(ARCH_PILOTO, ARCH_ESCUD, mostrarPiloto);
                if(flag==ERR_AP)
                    printf("\nNo se pudo abrir el archivo de carreras.dat o pilotos.dat");
                break;
            case '4':
                menuEstadisticas();
                break;
            case '5':
                menuExportarATXT();
                break;
            case '6':
                flag = funcionesABM(ARCH_PILOTO, ARCH_ESCUD, ARCH_CARRERA, cmpPilotoPorId);
                if(flag==ERR_AP)
                    printf("\nNo se pudo abrir algun archivo .dat");
                break;
            case '7':
                random = rand()%2;
                flag = simularCarrera(ARCH_PILOTO, (random==0) ? ARCH_CARRERA : ARCH_CARRERA2);
                if(flag==ERR_AP)
                    printf("\nNo se pudo abrir el archivo de carreras.dat!");
                else if(flag==ERR_MEM)
                    printf("\nNo se pudo reservar memoria!");
                break;
            case '8':
                char temp1[20] = ARCH_CARRERA;
                char temp2[20] = ARCH_CARRERA2;
                char resultado[20] = "combinado.dat";
                //ingresarNomCarrera(temp1);
                //ingresarNomCarrera(temp2);
                flag = combinarCarreras(temp1, temp2, resultado);
                if(flag==ERR_AP)
                    printf("No se pudieron abrir los archivos de origen.\n");
                else if(flag==TODO_OK)
                    printf("Temporadas combinadas con exito en %s!\n", resultado);
                break;
            case '0':
                printf("\n\t ---- GRACIAS POR USAR EL GESTOR DE F1 ----\n");
                break;
        }
        if(op!='0'){
            printf("\n");
            system("pause");
        }
    }while(op!='0');
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
    *aux = '\0';
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

int cmpPilotosPorPuntos(const void* a, const void* b){
    tPiloto* pilotoA = (tPiloto*)a;
    tPiloto* pilotoB = (tPiloto*)b;
    return pilotoB->puntos_acumulados - pilotoA->puntos_acumulados;
}

void mostrarPiloto(const void* v){
    tPiloto* pil = (tPiloto*)v;
    printf("ID: %d | Nombre: %-20s | Puntos: %d \n", pil->id, pil->nombre, pil->puntos_acumulados);
}

int cmpPilotoPorId(const void* clave,const void* registro){
    int id_buscado = *(int*)clave;
    tPiloto* pil = (tPiloto*)registro;
    return id_buscado - pil->id;
}
